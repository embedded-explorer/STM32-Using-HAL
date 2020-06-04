<h2 align="center">ARM Semihosting</h2>
ARM Semihosting is another cool feature which allows to use printf like statements to debug the application.

#### Follow these simple steps to enable semihosting

1. Set Linker Parameters<br>
Add a new flag under linker parameters
~~~
-specs=rdimon.specs -lc -lrdimon
~~~
2. Exclude syscalls.c from build

3. Set Debug Configuration<br>
Select ST-Link(OpenOCD) under debugger and add Run Command
~~~
monitor arm semihosting enable
~~~
4. include initialise_monitor_handles function
~~~
extern  void initialise_monitor_handles(void);
initialise_monitor_handles();
~~~

