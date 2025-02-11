# STAR Workshop: Git

## Tasks  

1. Enable scripts in Ubuntu  

   ```bash
   chmod +x change*.sh
   ```

2.Initialize a Git repository
3.Save the first version with the tag v1.0.0
4.Run the script change1.sh
5.Save the second version with the tag v1.1.0
6.Run the script change2.sh
7.Save the third version with the tag v1.2.0
8.Create an alternative branch Alternativa
9.Run the script change3A.sh
10.Save the fourth version with the tag v1.3.0
11.Switch to the branch Alternativa
12.Run the script change3B.sh
13.Save the fifth version with the tag v2.0.0
14.Generate a development graph (with an improved log)
15.Compare versions v1.0.0 and v1.1.0
16.Compare versions v1.2.0 and v1.3.0
17.Compare versions v1.1.0 and v2.0.0

--------------------------------------------
Git taháky:

* [Git Cheat Sheet](https://github.github.com/training-kit/downloads/github-git-cheat-sheet.pdf)
* Initialize a repository:

``` bash
git init
```

* Add a file to the staging area:(stage):

``` bash
git add <filename>
```

* Create a commit:

``` bash
 git commit -m "my message"
 ```

* Create a tag:

``` bash
 git tag <tag>
 ```

* Alias for a better log view::

``` bash
git config --global alias.prehled "log --oneline --decorate --graph --all"
```

* Create a branch::

``` bash
git branch <branchname>
```

* Switch branch:

``` bash
git checkout <branchname>
```

* Compare versions:

``` bash
git diff <tag1> <tag2>
```
