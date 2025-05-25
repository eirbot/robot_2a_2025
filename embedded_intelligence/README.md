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

## Configuration du robot

Le programme utilise un fichier `.toml` comme ceux définis dans
`./robot_settings/` pour se configurer, notamment au niveau de son mode de
communication et de son niveau d'affichage de logs pour pouvoir travailler dans
plusieurs environnement d'exécution en phase de debugging ou en phase de
production.

Le fichier est à mettre en paramètre du programme python

```sh
poetry run <script-poetry> --config ./robot_settings/debug.toml
```

Une série de couple `(script-poetry, configuration)` a été définie dans le
`justfile` pour exécuter le programme du robot dans plusieurs contextes
différents.

### Configuration de la communication serial

Cet automate communique avec la carte STM-32 via un port série qui est à
définir dans le champ `robot.communication.ports` du fichier `.toml`

```toml
[robot]

# reste de la configuration du robot...

[robot.communication]
protocol="serial"
ports="/dev/ttyUSB0"

# reste de la configuration du robot...
```

## Exécuter le script principal du robot

L'architecture de ce projet n'utilise que des modules python en *mode package*.
Poetry les exécute automatiquement en tant que module avec des scripts
prédéfinis:

```sh
# se place dans l'environnement virtuel de poetry et exécute le module main
poetry run robot-main-loop --config <fichier-config.toml>
```

En production, le robot devra tourner avec le fichier
`./robot_settings/main.toml` :

```sh
just main_loop_for_robot
# ou bien $ poetry run robot-main-loop --config ./robot_settings/main.toml
```

## Tester la communication serial

Sur Linux, il est possible avec l'outil `socat` d'ouvrir un *pont serial
virtuel* pour faire communiquer deux processus entre eux via serial.

Avec cela, une boucle de stratégie artificielle du robot a été programmée
accompagnée d'un programme imitant en serial la réception et l'envoi
d'information par l'esp. Pour exécuter et tester la bonne communication entre
ces deux processus, suivre les étapes suivantes :

1. Ouvrir le pont

   ```sh
   just start_virtual_serial_bridge
   # ou bien
   socat -d -d pty,raw,echo=0 pty,raw,echo=0
   ```

2. Modifier les fichiers de configuraton `./robot_settings/debug.toml` et
   `./robot_settings/debug_mock.toml` avec, pour chacun, un des 2 ports séries
   proposés par `socat`.

3. Exécuter l'imitateur de l'esp

   ```sh
   just mock_serial_run
   # ou bien
   poetry run serial-mock-esp --config ./robot_settings/debug_mock.toml
   ```

4. Enfin, exécuter la boucle de test du robot (essayer de faire ça rapidement
   après l'étape 3 si on veut que la communication se déroule sans timeout) :

   ```sh
   just debug_loop_for_robot
   # ou bien
   poetry run test-main-loop --config ./robot_settings/debug.toml
   ```

### Tester la communication avec autre chose que serial

Pour tester la boucle de communication duplex de manière générique, il est
proposé d'effectuer le même test, mais avec autre deux tubes nommés (fifo) (ça
se passe toujours sur Linux, hélas).

```sh
just mock_fifo_run
# ou bien
poetry run fifo-mock-esp --config ./robot_settings/fifo_debug.toml
```

Puis exécuter la boucle du robot dans le processus suivant:

```sh
just debug_fifo_loop_for_robot
# ou bien
poetry run test-main-loop --config ./robot_settings/fifo_debug.toml
```

## TODO

Gérer en priorité ces tâches :

- [ ] gérer les canettes pour les gradins
- [ ] monter un étage
- [ ] poser les bannières
- [ ] partir et garer le robot à la fin

Leur spec est [ici](https://www.eurobot.org/wp-content/uploads/2024/10/Eurobot2025_Rules.pdf)
