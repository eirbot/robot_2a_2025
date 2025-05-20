# API entre la Rasp et l'ESP

```cpp
void ComWithRasp::processCommand(const String& cmd, const std::vector<int>& params) {
    if (cmd == "G" && params.size() == 3){
        Serial.println("GoToPosition");
        serialGoto.Go((float)params[0], (float)params[1], (float)params[2]);
    } else if (cmd == "C") {
        Serial.println("Cannette");
        pousserCanettes();
    } else {
        Serial.println("Commande inconnue");
    }
}
```
