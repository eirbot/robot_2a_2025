# Coupe de France 2025 2A : logiciel du robot

## Code bas-niveau embarqué

Dans le dossier [`embedded_firmware/`](./embedded_firmware/) se trouve tout le
code bas-niveau interagissant avec les capteurs et les moteurs pour effectuer
les tâches suivantes :

- réaliser à la demande un goto <!-- TODO: spécifier le goto -->
- réaliser à la demande un pivotement
- réaliser à la demande une attente
- interrompre à la demande la tâche en cours de réalisation
- avertir de la terminaison de la tâche à réaliser (succès ou échec)
  <!-- TODO: voir si une notification d'échec est possible avec une
  transmission d'informations dessus; définir la spec à ce propos -->
- transmettre régulièrement des informations de capteurs
- transmettre à la demande des informations de capteurs

## Code haut-niveau de stratégie embarqué

Dans le dossier [`embedded_intelligence/`](./embedded_intelligence/) se trouve
le code haut-niveau qui traitera les informations suivantes :

- données sur le plateau encodées en dur dans le robot
- données environnementales en temps réel communiquées par un autre logiciel

devra établir un état du jeu et exécuter une stratégie pour l'améliorer à son
avantage.
