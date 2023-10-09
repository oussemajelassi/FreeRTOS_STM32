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

Noting my program was stuck in ``configASSERT( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 );`` , meaning that freertos did not confirm that this section is allocated he was right because the pointer pointed some where else.
I solved the problem by using ``strcpy (Heap_AllocatedMemory , RTOS_USART_ORDERS_ch) ; ``

