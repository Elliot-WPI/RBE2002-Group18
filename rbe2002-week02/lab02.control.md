# Speed Control

Individually, each team member will create a wheel speed controller. As a team, you will discuss each student's solution and commit one of them to your project library. Your team will then implement a navigation algorithm with your shiny, new speed controller.

Git allows you to take snapshots of your code and reverse changes in case they do not work. Therefore it is important to make as many snapshots as necessary to work efficiently. Of course, Git loses its meaning when only refined code is being committed and pushed. **If you do all your work and then push it with a single commit, you risk earning a low grade.** Get in the habit of making a commit every time you make a significant change to your code.

We give example instructions for Git using the command line interface, but you're welcome to use the VSCode interface.

### Step 1: Create a personal branch

Each group member will have to program their own version of a **speed controller**. Create your own branch from the group repository. As a branch name, use your first dash last name "first-last" (e.g., isaac-asimov). Use VSCode or the CLI:

```
git checkout -b "your-branch-name"
git branch
```

The terminal will confirm that you are in your own personal branch. 

### Step 2: Create a file in the user folder

Each group member must create an `md` file and name the file after their first and last names. You can create the file and commit it through VSCode or use the command line interface (CLI) by opening a terminal and entering:
```
git pull
cd users
touch first-last.md
git add --all 
git commit -m "I added my user id and name file"
git push
```

(Some machines may not have the command `touch` -- just create the file in the directory by whatever means you wish.)

> <span style="color:red"> Add a line of code to your markdown file:</span>

```
## This is my version of the assignment.
```
Now add, commit, and push the branch to your group repository:

```
git add -A
git commit -m "this is my own version of the git repository."
git push -u origin "your-branch-name"
```

Check your group's repository in a browser. You should be able to see the repository and a branch with your name (you may need to refresh the page).