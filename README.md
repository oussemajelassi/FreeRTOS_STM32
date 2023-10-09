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

![image](https://github.com/oussemajelassi/FreeRTOS_STM32/assets/100140668/3a7e9078-dc78-4d90-b1bd-eb33fe3a0f6e)

The task is meant to finally change the value of a global Variable nammed **MPU_SAMPLING_RATE**.
The other task which is the main task will obviously be sent to a **Blocked State** for a period equal to the MPU_SAMPLING_RATE.

I think that there is another good way to implement the listening state inside the communication task which is to make a semaphore and send the main task to a Blocked state until User ends with updating sampling rate.
I will get back to this when I learn about Semaphores

