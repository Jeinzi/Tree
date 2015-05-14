Tree
====

Using the windows terminal, one may graphically list a directory structure using the *tree* command.
Some years ago, I had the desire to restore this structure so I could navigate more easily through it -
So this is what this fancy piece of code does!

*tree* without any additional parameters produces quite an ugly output when redirected to a file. (*tree >> myFile.txt*)
So the program expects the output of *tree /a [/f]* whereas
- /a enforces the use of ASCII characters.
- /f additionally lists all the files.

This produces an output like the following:
```
Folder PATH listing for volume XY
Volume serial number is DA2E-F542
C:\somePath\
|   .gitignore
|   readme.md
|   Tree.sdf
|   Tree.sln
|       
+---Release
|       Tree.exe
|       Tree.pdb
|       
\---Tree
        main.cpp
        Tree.txt
        Tree.vcxproj
        Tree_complete.txt
```

May you find a good use for the code :)