# Minesweeper Game using graphics.h
# Use only code::blocks compiler as Dev C++ has a bug which shows "Undefined Refernce to readimagefile()"
# Steps to run:
Only run the compiled application named "main.exe".
# Steps to compile "main.cpp"
1.  Create the project on the code::clocks after ading the prerequisite files
    of graphics.h namely: (a) graphics.h (b)libbgi.a (c) other file which is present in the folder to the mingw folder
2.  Add the copy and paste the the following linker commands to the linker options:
    -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
    and add the lbbgi.a to link libraries
 Or simply search for how to  add graphics.h in code::blocks in youtube it will be much simple :P 
 
# Important Note: if you compile without doing these things then it will not compile, also the main.exe(actual game) will also dissapear as it is linked to the main.cpp via "Minesweeper.dev" and you will have to download it again!!!
