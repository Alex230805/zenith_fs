---

---

# Zenith File System

### a light weigth filesystem created for 8bit micro computer and similar system that require some sort of file manager, but without any limitation


The main goal is to bring to old system a modern like piece of software that can simplify the file archiviation with a standard, simple and fast way to interact with an existing operating system and the bare metal under the hood.

## Requirement:

To be fair the only requirement are a standard c library. It's necessary to have a OS that can compile and run c program and can provide the basic standard function to the system. 

Any third party software that can provide those requirement will become fully capable to run Zenith FS.
A goal of mine is to provide a standard interaction layer and use the simplest standard functions that a standard libray can provide ( malloc(), free(), etc... ) and the standard tipes ( char, int, uint8_t ). 

The interaction layer ( as you can see from <a href="#image_1">Main structure</a> ) will function like a intermediary to excange operation from Zenith FS to the current OS. It's not allways a good idea to introduce a standard intermediary, expecially for speed goal, but this is a deal to mantain the highest compatibility possible, and to fast-fix Zenith for more strange and not-standard system. 


# <h1>Main Structure of Zenith</h1>

<img src="https://res.cloudinary.com/djjwizrmr/image/upload/v1725624599/zenith_fs/zenith_fs_main_structure.jpg" id="#image_1">