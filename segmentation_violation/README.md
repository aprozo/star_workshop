# STAR Workshop 2025: Git

# Indico
Prezentace: TBD

# Bugy

1. Špatný ukazatel na histogram
2. Špatný ukazatel na integer
3. Špatný ukazatel na string
4. Otevírání a zavírání souboru při tvorbě histogramu
5. Špatný počet parametrů pro funkci

--------------------------------------------
Alias pro GDB:

* Přidejte do ~/.bashrc
``` bash
pomoc() {
	gdb --quiet -ex run --args root.exe -l -b -q "$1++g"
}
```
* Debugging spustíte
``` bash
pomoc Bug_01_PoleHistogramu.C
```
