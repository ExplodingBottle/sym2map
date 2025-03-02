# sym2map conversion tool
*It is annoying when things don't work. It is even more annoying when it is because of a dumb reason.*
*But what if you wanted to use a SYM file, a symbol file for debug generated through `MAPSYM`, but it*
*didn't work because of being too old?*

*You would be very frustrated, having all these golden symbols*
*stuck in a really old file... Thanksfully, the frustration in this <u>very specific case</u> will no longer*
*be encountered thanks to __sym2map__ !*<br>

## What are SYM files?
SYM files are symbol files that use a binary format made by Microsoft and which was used a long time ago, before being replaced by
better symbol file technologies, such as __DBG__ (a file format that is the same as a PE executable and which contains only symbols)
or even the well known __PDB__ format (The <u>P</u>rogram <u>D</u>ata<u>B</u>ase file format which is used in MSVC nowadays).

Basically, using SYM files these days is just impossible, unless you download really old tools and run them inside of a virtual machine,
which is not a great idea as these tools will limit you.

## What are MAP files?
MAP files are some kind of text files which are readable by humans but also by tools. Their role is to explain how is divided the program
and describe where to find each symbols, or where is the entry point. You should generally find inside a MAP file these informations:
- Global informations (name of the module, address of the entry point, reallocation address)
- Informations about the sections (start address, length, name and type)
- Informations about the symbols (which segment at which offset, which name and which virtual address)

## What is sym2map?
sym2map is a debug file conversion tool which allows converting an old, dust-taking and unusable SYM file into a *human and computer readable*,
usable MAP file. You could even use different tools to then convert the MAP file into a recent debug file format.

The tool is capable of taking as input the symbol file at worst and both the symbol file and the binary file at best.
If you only supply the symbols file, you __will only be able to get the segment and offset addresses and the names of the symbols!__
The reason for this is that when a SYM file is created by the `MAPSYM` tool, it strips a lot of useful informations.

This is why you can also supply the binary executable or library (also called the image file in the tool) so you can also 
recover the address of the entry point, the best reallocation address, the layout of the segments/sections and the virtual
addresses of the different symbols (which is pretty useful when used to produce a MAP file for Ghidra)

## How to use?

___IMPORTANT NOTE: The program does not support 64-bits image files, as SYM files don't support the x64 architecture.___

This part comes at the end because it is one of the simplest tools to use: it is a command line tools that doesn't have options.
You just have to run the command like this:

`sym2map myfile.sym myfile.map myfile.dll`

Where `myfile.sym` represents your symbol file, `myfile.map` your output map file and `myfile.dll` your optional image file (it can
also be an exe file, the extension doesn't matter. The only thing that matters is that the image file is a PE executable for Windows NT)
In the case you don't have an image file, just run the command like this (same pattern as above):

`sym2map myfile.sym myfile.map`

## What is the license of this project?

__This tool is shared under the MIT License that you can check [here](LICENSE).__ But this only covers the code. __The other resources made with this program__
__(in fact the only resource, the icon) are available under the CC-BY-SA license available [here](CC-LICENSE)__
(the Icon has been made using GIMP)

## Any specification documentation for the SYM format?
All this work has been based on a very nicely made documentation (but certainly not much used) that is available on [the Windows Archeology wiki](https://win-archaeology.fandom.com/wiki/.SYM_Format).

However, if you reeally need a precise specification for this file format and with more informations (support for 32-bits segments), please check [the specification documentation included in this repository](SymSpec.md).

## Found a bug, a missing feature?
Don't hesitate to submit a bug report in the *Issues* section of this repository or even submit *Pull Requests* if you want to contribute
to this tool.

**Any contribution such as a bug report or a pull request would be greatly appreciated.**
