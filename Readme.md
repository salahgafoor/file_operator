## Steps for execution
Goto the root directory and execute

`make`

Insert loadable module into kernel

`sudo insmode file_operator.ko`

Write to module

`echo "This is file operator module" > /proc/file_operator_driver`

Read from module

`cat /proc/file_operator_driver`

Output:

`This is file operator module`
