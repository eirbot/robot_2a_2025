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

Si vous aimez taper une commande en 2ms dans le terminal grâce à
l'autocomplétion de la touche Tab, alors il est conseillé d'installer
`just` (et d'ajouter son autocomplétion dans bash). Un `justfile` a été défini
pour les amoureux des interfaces simples.

## Installation des dépendances

```sh
# En se plaçant dans le même dossier que ce README
poetry install
```

## Configuration du port série

Cet automate communique avec la carte STM-32 via un port série qui est à
définir dans l'instanciatoin de `SerialCom` située dans le constructeur de la
classe `Robot` du fichier `firmware.py`

## Exécuter le script principal du robot

L'architecture de ce projet n'utilise que des modules python en *mode package*.
Poetry les exécute automatiquement en tant que module avec des scripts
prédéfinis:

```sh
# se place dans l'environnement virtuel de poetry et exécute le module main
poetry run robot-main-loop
# pour les feignasses
# just main_loop_for_robot
```

## Tester la communication serial

Sur Linux, il est possible d'ouvrir virtuellement deux ports serial pour faire
communiquer ici deux processus de test. Pour cela suivre les étapes décrites
dans [`./protocol_for_serial_test.md`](./protocol_for_serial_test.md).

Ensuite, exécuter le programme déguisant l'esp :

```sh
poetry run serial-mock-esp
# just mock_serial_run
```

Puis exécuter une boucle de test du robot :

```sh
poetry run test-main-loop
# just debug_loop_for_robot
```

## Tester la communication sans serial

Sur Linux, il est possible de tester une boucle du robot avec une communication
locale utilisant une fifo plutôt qu'une communication serial. Pour cela,
exécuter dans un process à part

```sh
poetry run fifo-mock-esp
# just mock_fifo_run
```

Puis exécuter la boucle du robot dans le processus suivant:

```sh
poetry run test-fifo-main-loop
# just debug_fifo_loop_for_robot
```

## TODO

Gérer en priorité ces tâches :

- [ ] gérer les canettes pour les gradins
- [ ] monter un étage
- [ ] poser les bannières
- [ ] partir et garer le robot à la fin

Leur spec est [ici](https://www.eurobot.org/wp-content/uploads/2024/10/Eurobot2025_Rules.pdf)
