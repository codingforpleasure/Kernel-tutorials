Script files:
=============

* **checkpatch.pl** - Make sure the code you wrote follows the coding style of the kernel developers, otherwise it would notify 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;you. script was taken from the Linux kernel tree.
                  
                  Example: 
                  ./checkpatch.pl --no-tree --file ./<your source code file>

* **run_current_module.sh** - 
  This simple script which I wrote: 
  * clears the ring buffer
  * Inserts the module which resides in the current directory
  * prints the contents of buffer
  * removes the module from the linux kernel
                          
