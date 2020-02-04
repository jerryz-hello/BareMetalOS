# Building this milestone

To build this milestone, switch to the M3 directory (Scheduling) and type

```
make
make run
```

This should open the emulator with a prompt indicating the shell is running

# Optional features 

We did the following optional features:

* Quit command
* Print size as a number of sectors for dir command

To verify them,

Type `dir` and `quit` in shell, you should see a number after the filename indicating the size (number of sectors) that file takes. After you type quit, the emulator will stop receiving any input, which means that the shell has stopped running.