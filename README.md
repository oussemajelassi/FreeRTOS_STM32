# FreeRTOS_STM32
This will follow me while learning and implementing freeRTOS on the STM32

## How FreeRTOS sees things : 

FreeRTOS is very known of putting pieces of code in segment which he calls **Tasks**.
So, A Task is the elementary building bock of our code we use them to structure our **modular** code.
By default FreeRTOS creates a so called **Default Task**.
When creating task one thing to consider is the params we should give to the task : 

### How FreeRTOS organaizes its tasks : 

The very first Parameters are the Name, The priority and stack size.
**Stack Size** : Stack is a region of memory dedicated to a certain task. This region will have some informations needed suck as local data variables, function call informations and task execution context.
And **Function call informations** refers to data and state informations needed when the function within a task is preempted by another which is very frequent in our case.
However **Task execution context** focuses on the ocerall state of the task itself and defines its status and very needed when the task **yields** to another.

### Creating our first tasks : 

the **osThreadNew** which is the function used to create new tasks takes on 3 arguments which are : the function that the function will execute, A void* pointer if we want to pass arguments to the task function and a structure having the params we mentionned before. 

When using FreeRTOS we are almost always dealing with preemption and tasks giving CPU Access to each other so using the HAL_delay the blocking function is not permitted.
Instead we will be using **osDelay** which is non blocking and takes as arguments ticks number. To convert ticks into time we use simple math and the **TICK_RATE_HZ** param.

#### Controlling Sampling Rate Via a seperate Task without Interruptions : 

My First mini Project consisted of an **MPU6050** and a **UART** Communication. The Whole Idea is to capture the user New Order of measuring rate but without raising an interruption.
I managed to create twi task with the same Priority and with a simple **finite state machine** for the communication task in order to get to the final **'\r'** command.

![RTOS](https://github.com/oussemajelassi/FreeRTOS_STM32/assets/100140668/8a8d6213-bb77-4c07-86f2-d00a3da0a50b)

The task is meant to finally change the value of a global Variable nammed **MPU_SAMPLING_RATE**.
The other task which is the main task will obviously be sent to a **Blocked State** for a period equal to the MPU_SAMPLING_RATE.

I think that there is another good way to implement the listening state inside the communication task which is to make a semaphore and send the main task to a Blocked state until User ends with updating sampling rate.
I will get back to this when I learn about Semaphores

### Free RTOS : Memory Management : 

When Runnig a C Code, Memory is divided into several  parts noting : **Stack**, **Heap** and **Static**.
When Creating a Task in FreeRTOS We are allocating more space in the heap.

![autodraw 10_9_2023](https://github.com/oussemajelassi/FreeRTOS_STM32/assets/100140668/df66f011-bbd4-49ef-b51a-1236a406ca32)

As we see Both Stack and Heap are taking more and more memory and going towards each other Especially as every task takes on onther **TCB** and its own stack inside the Heap.
Following digikey's Tutorial I will try to implement the two tasks : One reading from UART and writing to heap the other reading from heap and freeing the allocated memory.
In addition I will not be using Priority manipulating as in First application However I will put **Notifications** into the action.

FreeRTOS Does not enable the use of the Standard Malloc Function However we can check **heap_x.c** Files for the provided and thread safe Functions we can use to access **Heap** Section inside RAM.

#### Task Notification : 

FreeRTOS enables an inter task communication mecanism called notification.
Basically I will but the task reading from heap into sleep, When ever I complete recieving the text from user I will wake it up using provided API.

**xTaskNotify()** and **ulTaskNotifyTake()** are used in my example in order to alert task 2. 

When using ulTaskNotifyTake we can choose to clear or not the notification and that using the first parameter : setting it to either **pdTRUE** or **pdFALSE**.

#### Heap Memory corruption Fixed : 

Earlier in the code I was affecting directly the buffer to the Heap_AllocatedMemory like this : 

``Heap_AllocatedMemory = RTOS_USART_ORDERS_ch ``
That thing resulted in a corruption since there is no data transfer how ever the pointer **Heap_AllocatedMemory** is now pointing to another adress.

Noting my program was stuck in : 
``configASSERT( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 );``  

meaning that freertos did not confirm that this section is allocated he was right because the pointer pointed some where else.
I solved the problem by using : 

``strcpy (Heap_AllocatedMemory , RTOS_USART_ORDERS_ch) ; ``

#### Side Task debugging : C pointers : 

While Taking on the "Memory management application I faced something weird after reading The user Input I wanted to allocate dynamic memory and read it from another task after it get notified.
However For every input I only recieve only the first four chars, After a while of debugging I figured what went wrong :
``char* Heap_AllocatedMemory ;``

I used this pointer later like this : 

``HAL_UART_TRANSMIT(xxxx , xxxx ,  **sizeof ( Heap_AllocatedMemory )**, xxx ) ; ``

And Yes, Heap_AllocatedMemory is a pointer which size of course 4.

I fixed this by simply using 

``HAL_UART_TRANSMIT(xxxx , xxxx ,  **strlen ( Heap_AllocatedMemory )**, xxx ) ; ``


### Queues : Data Transfer : 

Queues are a First in First Out data structure and are used in freeRTOS in order to communicate between task.
The basic Idea is that one task is going to put data inside while the other is going to read : 

![image](https://github.com/oussemajelassi/FreeRTOS_STM32/assets/100140668/4189202c-9464-49c6-af3e-8da9119d6975)

In case two tasks are reading at the same time The one that have more priority will be served first.

In case One task tries to read from an empty queue it will go to a **suspended** state until that queue is filled.

Final results are shown here : One task reads from UART, fills a queue and the other task unbox the queue and update its toggle rate.
I think Queues still have many features for me to discover however for now that ll be it.

https://github.com/oussemajelassi/FreeRTOS_STM32/assets/100140668/05745b59-1af5-4ab3-8c5a-8d255e6f57be

### Ressources management : 

#### Basic Critical Sections : 

Critical Sections in FreeRTOS are lines of code in which we wont accept other task taking on the CPU.

To ensure this we need to put these line between two macro-like functions which are : 

``taskENTER_CRITICAL()`` and ``taskEXIT_CRITICAL()``
They work in a very simple way as they disable interruptions so there will be no way of context switching.

#### Suspending the Scheduler : 

Instead of disabling all interrupts which is sometimes not possible we can disable the schedular and leave interrupts working so that every context switching demand is kep on hold until the schedular comes back to life again.
We can achieve this by simply calling  ; 
``vTaskSuspendALL() ;`` and then ``vTaskResumeALL();``

#### Mutexes : 

Mutexes are the most known and used in this context they are seen as a token or a trophy that enables a task to use a shared ressource that can be anything like a variable or a peripheral.

This Challenge will be nothing but a circular buffer that gets filled with numbers from "Producer Task" and get read from a "Consumer Task".
I will try to shape this so I can Practice C++ Programming a little bit and combine twwo powers into one.

The application combines using a shared mutex in order to get access to the buffer, every producer can insert until free spots are done or until it inserts or its production.
Every Consumer can withdraw from the buffer until there is no filled spots or if its needed quantity is satisfied.
