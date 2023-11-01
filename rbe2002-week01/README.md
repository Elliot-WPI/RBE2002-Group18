# Lab assignment: Cloning libraries and example code

The bulk of Lab 1 is covered in a handout. To complete the handout, however, you will need some libraries and sample codes. In general, each week we'll provide skeleton codes and libraries, and you'll customize and implement the libraries to add new functionality to your Romi.

**Because you will be editing libraries that we provide, you will need to make local copies on your machine.** You will use `lib_extra_dirs` to link to them in platformio.

For this lab, you will be introduced to Git and Github and _clone_ some libraries.

## Step 0: Get set up with Git and Github

To manage code you will be using Git and GitHub. Here is your opportunity to learn about _version control_, which will improve your performance in this course and your employability.

To get you started with Git, consult the following resources:
* Get set up with [Git for VSCode](https://code.visualstudio.com/docs/sourcecontrol/overview). If you need to install Git, use [this link](https://git-scm.com), referred to in the previous video.
* Before next week, take [a deeper look into Git and Github](https://www.youtube.com/watch?v=RGOj5yH7evk). Take your time to understand the concepts, though you should already be set up by the time you watch it. In particular, the video explains how to use the command line interface (CLI) for Git.
* You may want to install other Git tools. Your professor uses GitHub Desktop extensively. VSCode has other tools that you may want to explore.





## Step 1: Clone this repository to your computer

Clone this repository to your local machine. At the command line, you can type

```
git clone <PASTE THE REPO ADDRESS HERE>
```

Alternatively, you can clone it from within VSCode. Or use GitHub Desktop.

**Anectdotally, we have seen that cloning a repository to a folder linked to OneDrive causes issues.** The two do not seem to play nicely together. In particular, if you clone to your Documents folder, be sure that it is not linked to a OneDrive account. You do **not** need cloud storage via OneDrive -- that's what GitHub is for!

The repository contains example codes, one for each of the MaxBotix interfaces. **You will also need to clone the set of libraries**, provided in another repo.

## Step 2: Open a project folder in VSCode

Open one of the project folders. Note that **when opening folders in VSCode, you must open them at the Project level** (*not* the repository level).

Each student should "drive" for one of the examples. To avoid merge conflicts (we'll discuss those another day), you should do each of the three interfaces serially: Student A clones, edits, and pushes. Then Student B does the same. Then Student C. It's not _quite_ the spirit of Git, but if you're just getting started with Git, it's safer. Plus then you will all work together on the interfaces.

## Step 3: Point platformio to your local directories
You should be familiar with using `lip_deps` for including libraries in platformio. `lib_deps` works well for stable libraries, such as `IRDecoder` (which you will not have to edit in this course). When you use `lib_deps`, platformio makes a _temporary_ copy of the library on your computer. You can edit it, if you wish, but your changes will be clobbered if you ever download the library again.

For your development work, you'll want to use `lib_extra_dirs`, which points platformio to a _local_ directory on your computer. Since your code was obtained using Git, you will now have full version control for any changes you make to the libraries (which will happen frequently).

We have set the repo up using relative paths, which should work fine, so long as you don't move directories around.

## Step 4: Upload a program to your Romi

Return to the Lab Worksheet, upload the code, and answer the questions.
