---

---

# Zenith File System

### a light weigth filesystem created for 8bit micro computer and similar system that require some sort of file manager, but without any limitation


The main goal is to bring to old systems a modern like piece of software that can simplify the file archiviation with a standard, simple and fast way that interact with an existing operating system, and the raw metal under the hood.

## Requirement:

To be fair the only requirement are a standard c library. It's necessary to have a OS that can compile and run c program and can provide the basic standard function to the system. 

Any third party software that can provide those requirement will become fully capable to run Zenith FS.
A goal of mine is to provide a standard interaction layer and use the simplest standard functions that a standard libray can provide ( malloc(), free(), etc... ) and the standard tipes ( char, int, uint8_t ). 

The interaction layer ( as you can see from <a href="https://res.cloudinary.com/djjwizrmr/image/upload/v1725789120/zenith_fs/bddxpzq7pbr98cff9yhh.jpg">Main structure</a> ) will function like a intermediary to excange operation from Zenith FS to the current OS. It's not allways a good idea to introduce a standard intermediary, expecially for speed goal, but this is a deal to mantain the highest compatibility possible, and to fast-fix Zenith for more strange and not-standard system. 


# <h1>Main Structure of Zenith</h1>

<img src="https://res.cloudinary.com/djjwizrmr/image/upload/v1725789120/zenith_fs/bddxpzq7pbr98cff9yhh.jpg" id="#image_1">


# Types of supported drive: it's may depend ...


Due to the nature of the filesystem write a standard way to write and read things from drive is nearly impossible. 
The nature of the Operating System where Zenith were be compiled is unknown 
and, beside a standard way, for other types of support like the serial ones
or other things is necessary to write it dow from scratch.

Well in reality in any complete Operating System with the goal of storing and archiving things, a driver for each single external support is obviously present. So the structure of the I/O layer of Zenith is dependent by those driver. 

With a selection is possible to change the method used for write or read, and
it's a simple switch case. 

The default implementation is a simple parallel connection with the address bus and it's possible to add newest ones by add a new case in the switch and 
simply integrate the writing and readign using standards kernel's endpoint.


<strong>Uncomment the necessary pre-compiler constant to enable the default support or to compile in Developer mode.

Those are located on top of the zenith.h header file  
</strong>

### for more information check out the zth_dev_in.h header library


## Initialize a new filesystem

Do tue the nature of the phisical machine where zenith will be installed it's not possible 
to generate a new filesystem directly from the running 8bit machine because of the size of
the fstab that would occupy the entire RAM memory if you try to create a very big partition.
That's why when zenith it's compiled without the <strong>VIRTUAL_DRIVE</strong> precompiler 
directive <strong>zenith_initfs()</strong> would be built to only run on a modern computer. 


zenith_initfs() will build a binary file with the partition table written inside ready to be 
flashed in a ROM or in any support adressed by <strong>zenith_push()</strong> and <strong>zenith_pop()</strong> ( so the interface layer from zenith and the Kernel/machine ).


The other function would be accessible by a running 8bit system without problems.

#### For that it's possible to exclude from the compilation zenith_initfs() to generate a smaller binary.
