# Liquid Ring Noise - Liquid noise ring background

Make a liquid ring noise background with or without color using GEGL

![image](https://github.com/user-attachments/assets/d0d190a1-10de-42d3-b71c-518be91fcbd6

![image](https://github.com/user-attachments/assets/b8da05e2-c0f4-4a0d-98d6-595f4fdad74e)




## Directory to put Binaries (They do NOT go in the normal plugins folder)

**Windows**
C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins

**Linux** 
`~/.local/share/gegl-0.4/plug-ins`

**Linux (Flatpak)**
`~/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`


Then Restart Gimp and go to Filters>Noise and look for "Liquid Noise" or just search for it with /

GIMP 2.10 users will only find this in GEGL Operation drop down list and it will only work on 2.10 if they are using GEGL 0.4.50+, all normal windows builds of GIMP 2.10 ship with GEGL 0.4.48

## Compiling and Installing

Just run build_plugin_linux.sh or build_plugin_windows.sh 

or manually

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## More previews of this based plugin 

![image](https://github.com/user-attachments/assets/fa6562c6-237b-4915-b7f0-f907179e9739)

![image](https://github.com/user-attachments/assets/1f376425-3699-405f-b067-0f9f7e67819f)

![image](https://github.com/user-attachments/assets/a497cfc9-d21f-4e62-b313-76bba30630d4)



