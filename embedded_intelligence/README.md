# Intelligence embarquée

Ici se trouvera le code de l'automate de stratégie du robot qui sera embarqué
dans une carte de type Raspberry Pi.

## Requirements

Il est attendu d'avoir la version `3.11.2` de Python installée sur la machine.

Pour la gestion du projet, de ses dépendances et de son environnement
d'exécution, l'outil moderne Poetry (`>=2.1`) est utilisé.  
Sur un Raspberry OS fraichement installé, attention à d'abord mettre à jour
toutes les dépendances systèmes avec `apt`

```sh
sudo apt update
```

Ensuite, poetry peut être installé avec cette commande, provenant du [site
web](https://python-poetry.org/docs/#installing-with-the-official-installer)

```sh
curl -sSL https://install.python-poetry.org | python3 -
```

## Installation des dépendances

```sh
# En se plaçant dans le même dossier que ce README
poetry install
```

## Configuration du port série

Cet automate communique avec la carte STM-32 via un port série qui est à
définir dans la variable `PORT` du fichier `esp_serial.py`

## Exécuter le script principal du robot

L'architecture de ce projet n'utilise que des modules python en *mode package*.
Poetry les exécute automatiquement en tant que module avec des scripts
prédéfinis:

```sh
# se place dans l'environnement virtuel de poetry et exécute le module main
poetry run robot-main-loop
```


## TODO

Gérer en priorité ces tâches :

- [ ] gérer les canettes pour les gradins
- [ ] monter un étage
- [ ] poser les bannières
- [ ] partir et garer le robot à la fin

Leur spec est [ici](https://www.eurobot.org/wp-content/uploads/2024/10/Eurobot2025_Rules.pdf)
