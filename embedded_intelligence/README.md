# Intelligence embarquée

Ici se trouvera le code de l'automate de stratégie du robot qui sera embarqué
dans une carte de type Raspberry Pi.

## Requirements

Avoir la version `3.11.2` de Python installée sur la machine.

## Installation des dépendances

```sh
# $PYTHON désigne l'exécutable de python 3.11.2
$PYTHON -m venv eirbot_venv
source eirbot_venv/bin/activate
pip install -r requirements.txt
```

## Configuration du port série

Cet automate communique avec la carte STM-32 via un port série qui est à
définir dans la variable `PORT` du fichier `esp_serial.py`

## Exécuter le script expérimental de communication full-duplex

Si ce n'est déjà fait, sourcer le terminal dans l'`eirbot_venv`

```sh
source eirbot_venv/bin/activate
```

Enfin, lancer le programme

```sh
python esp_serial.py
```

## Exécuter l'automate

Si ce n'est déjà fait, sourcer le terminal dans l'`eirbot_venv`

```sh
source eirbot_venv/bin/activate
```

Enfin, lancer le programme

```sh
python main.py
```

## TODO

Gérer en priorité ces tâches :

- [ ] gérer les canettes pour les gradins
- [ ] monter un étage
- [ ] poser les bannières
- [ ] partir et garer le robot à la fin

Leur spec est [ici](https://www.eurobot.org/wp-content/uploads/2024/10/Eurobot2025_Rules.pdf)
