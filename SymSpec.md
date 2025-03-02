# SYM File technical specification
This specification has been made with the help of informations that we could find on [the Windows Archeology wiki](https://win-archaeology.fandom.com/wiki/.SYM_Format), as well as a little bit of tinkering with `MAPSYM` by trying to input hand-made MAP files to check the resulting SYM files.

*There are two reasons for making this specification*:
- There is too little to no informations available about this file format, so it is a good idea to share more informations for everyone.
- In a certain repository it was written that `source code is the ultimate documentation`, to which I don't agree. An ultimate documentation/specification should
be efficient, and trying to read lines of code just to understand a specification is really not worth it.

> ___The online documentation___ will refer to the Windows Archeology wiki

## Endianness note
__The values must be read in little endian.__

## Notion of paragraphs
In a SYM file, the reference to different parts of the file can either be done with paragraphs (in newer formats) or bytes (in older format) according
to the online documentation.

A file in the newer format consists in blocks of 16 bytes (0x10 bytes). For this reason, you will find padding null bytes at some places in a file in the newer format.

|           | 0x0 | 0x1 | 0x2 | 0x3 | 0x4 | 0x5 | 0x6 | 0x7 | 0x8 | 0x9 | 0xa | 0xb | 0xc | 0xd | 0xe | 0xf | Paragraph Number |
|:---------:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|-----------------:|
| 0x0000000 | H   | e   | l   | l   | o   |     | W   | o   | r   | l   | d   | !   |     | P   | a   | r   | 0x00             |
| 0x0000010 | a   | g   | r   | a   | p   | h   |     | t   | e   | s   | t   | .   | .   | .   |     |     | 0x01             |

Depending on the version of `MAPSYM` that generated the file, it will use paragraphs or bytes. You can use the version to determine whether
you should use paragraphs or bytes. Another way could be to use the size contained in the header (because it is either in bytes or in paragraphs):

__Note that you need to subtract the 4 first bytes of the file because they are excluded from the size computation because they themselves tell the size__

```
# Pseudo-code
fileSize (size type) = getFileSize();
sizeInHeader (unsigned 16 bits) = read16BitsValueInFile()

if fileSize - 4 == sizeInHeader:
    oldFormat = true # Size is in bytes, we use bytes
else if (fileSize - 4) / 0x10 == sizeInHeader:
    oldFormat = false # Size is in paragraphs, we use paragraphs
else:
    fail("Size mismatch") # Size is wrong, probably not a SYM file
```

*According to the online documentation, `MAPSYM` version 2.08 to 3.00 generates files which use bytes and version 3.10 and greater generates*
*files which use paragraphs instead.*

## File header
The file header as a quite simple structure. Here are the different values you can extract from it (values are all unsigned):
|   Size     | Explanation                                                                             |
|:----------:|:----------------------------------------------------------------------------------------|
| 32 bits    | This value represents the size of the symbol file, either in bytes or in paragraphs.<br>__This value excludes itself from the count, so add four bytes to this value when comparing to the file size__|
| 16 bits    | The number of the segment containing the entry point (but no address) |
| 16 bits    | This value tells how much symbols you will find in the segment zero (it can just be 0) |
| 16 bits    | This value tells in bytes the size of the header __including the 32 bits length__ and __all the content of the zero segment__<br>Basically, it goes from the beginning of the file to the start of the first segment |
| 16 bits    | This value tells how much segments you will find in the symbol file, excluding the zero segment |
| 16 bits    | Address in the symbol file of the first segment. It is either in bytes or paragraphs.<br>You will need to seek at this address |
| 8 bits     | *Unknown* |
| 8 bits     | Size in bytes of the module name that will come after. The size is the exact size, there is no null character |
| 8n bits    | The name of the module in ASCII (with n = the 8 bits value found above) |
|__8/0 bits__| This byte seems useless, but need to be taken in account to avoid creating an offset. It is only present when the file uses bytes and not paragraphs |

Then comes the segment zero without any kind of padding. It is a sequence of the following data repeated n times where n
corresponds to the 16 bits value telling how much symbols will be found in segment zero:
|   Size     | Explanation                                                                             |
|:----------:|:----------------------------------------------------------------------------------------|
| 16 bits    | The address of the symbol. In assembly this should be rather called offset as this values corresponds to n in 0000:nnnnnnnn |
| 8 bits     | Size in bytes of the symbol name that will come after. The size is the exact size, there is no null character  |
| 8n bits    | The name of the symbol in ASCII (with n = the 8 bits value found above) |

__After finishing to read the header, please jump to the address of the first segment contained in this header and described above.__

## Segments
After the file header will be a series of segment. It is important than you jump from one segment to the other using the informations
communicated by the segment and not trying to continue to read.
|   Size     | Explanation                                                                             |
|:----------:|:----------------------------------------------------------------------------------------|
| 16 bits    | This value represents the address of the next segment in the symbol file, either in bytes or in paragraphs<br>__This is the value you need to use to jump to the next segment after reading the current one__|
| 16 bits    | This value tells how much symbols you will find in the current segment |
| 16 bits    | This tells the size in bytes of the segment.<br>__If the symbol file uses bytes and not paragraphs, this value is the address in bytes of the end of the segment__ |
| 16 bits    | This tells the segment number n which corresponds in assembly to nnnn:aaaaaaaa where a is an arbitrary value |
| 6*8 bits   | *Unknown* |
| 8 bits     | This value informs us on the nature of the segment. We can assume that it is only non-null if the segment is 32 bits and null if in 16 bits |
| 5*8 bits   | *Unknown* |
| 8 bits     | Size in bytes of the segment name that will come after. The size is the exact size, there is no null character  |
| 8n bits    | The name of the semgent in ASCII (with n = the 8 bits value found above) |

After this segment header, you will find the symbols contained in the segment. The number of symbols that will come are indicated
by the 16 bits value described above.
|   Size     | Explanation                                                                             |
|:----------:|:----------------------------------------------------------------------------------------|
| 16/32 bits | The address of the symbol. In assembly this should be rather called offset as this values corresponds to n in 0000:nnnnnnnn<br>__If the segment is a 32 bits segment, then the size of this value is 32 bits. Else, the size of this value is 16 bits__ |
| 8 bits     | Size in bytes of the symbol name that will come after. The size is the exact size, there is no null character  |
| 8n bits    | The name of the symbol in ASCII (with n = the 8 bits value found above) |

__After finishing to read the segment, please jump to the next one using the first 16 bits value found in the segment header.__
__Don't attempt to continue reading as you may fall or not on garbage data__

At the very end of a segment, there seem to be one trailing byte that has an unknown usage.

## Version
At the very end of the file, the two last bytes represent the version of `MAPSYM` used to create the file.

## Important notes
There are several traps while trying to parse the SYM file. Here is a list of these in which you could fall:
- In the file header, you could forget that you must __skip one byte before reading the symbols of the segment zero if the file uses bytes__.
- You could want to read the file in a linear way. This is a bad idea, because it seems that there is garbage data between segments.
Instead, after finishing to read a segment, __you must jump to the next one by using the address of the next segment it contains__
instead of continuing to read.

## Code snippets
Here could be a nice way for you to implement the segment parsing loop:
```
# Pseudo-code
nextSegmentAddress (unsigned 16 bits) = firstSegmentAddress
for iterator = 0, do while iterator < segmentsNumber:
    if oldFormat:
        fileGotoByte(nextSegmentAddress)
    else:
        fileGotoByte(nextSegmentAddress * 0x10)
    
    nextSegmentAddress = read16BitsValueInFile()
    ...
    ... parse the segment
    ...
   
```