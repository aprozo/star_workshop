# STAR Workshop 2025

## Indico

Indico event: [Indico link](https://indico.fjfi.cvut.cz/event/339/)

## STAR Hackathon 2025 Handbook

This year, all presentations will be compiled into a single PDF. Everyone presenting will be required to transcribe their presentation into the shared PDF:  
[Overleaf link](https://www.overleaf.com/4491921739vyyprzgxxmzb#656cf6)

## How to Work with This Repository

This repository is used for sharing code among workshop participants. Each collaborator has their own folder where they can add their files. Below is the recommended procedure for adding new changes.

### 1. Cloning the Repository

If you don't have the repository on your device yet, clone it:

```bash
git clone git@github.com:aprozo/star_workshop.git
cd star_workshop
```

### 2. Updating Before Starting Work

Before making changes, fetch the latest version of the repository:

```bash
git pull origin main
```

### 3. Making Changes

Modify files only within your folder. After making changes, save the files and add them to the commit:

```bash
git add <path_to_file>
git commit -m "Description of change"
```

### 4. Pushing Changes to GitHub

Before pushing your changes, ensure you have the latest version of the repository:

```bash
git pull origin main --rebase
```

If there are no conflicts, you can push your changes:

```bash
git push origin main
```

Original private repository: [https://github.com/Lomicond/STARWorkshopCZE2025](https://github.com/Lomicond/STARWorkshopCZE2025)
