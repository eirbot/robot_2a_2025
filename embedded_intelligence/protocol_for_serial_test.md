# Test sur Linux de la communication serial

## Lancer `socat

```sh
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

Ceci va rendre disponbile une communication sur les ports serial

- `/dev/pts/3`
- `/dev/pts/4`

Bien lire dans la sortie de socat lesquels seront disponibles.

Interrompre avec Ctrl D

## Lire et écrire sur un port série avec `screen`

```sh
# 9600 définit le baudrate
screen /dev/pts/4 9600
```

Pour arrêter la lecture et l'écriture, Ctrl-A, puis D, puis

```sh
screen -ls
```

On copy l'id de session dans `$SESSION_ID`

```sh
screen -X -S $SESSION_ID quit
```

## Lire et écrire sur un port série avec la loop du robot

Définir `SERIAL_PORT` dans `serial_com.py` à `/dev/pts/3`, garder le baudrate à
9600 puis lancer la loop du robot:

```sh
poetry run robot-main-loop
```
