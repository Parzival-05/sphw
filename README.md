# SPHW

Задача: файловый ввод/вывод без использования pipes


Cборка: 
```
make clean && make
```

Тест:
```
./pong 3 & sleep 0.2; ./ping 3
```

Вывод:
```
[1] 6189
[PING] Записал данные: Message 1
[PONG] Прочитал данные: Message 1
[PING] Записал данные: Message 2
[PONG] Прочитал данные: Message 2
[PING] Записал данные: Message 3
[PONG] Прочитал данные: Message 3
[1]+  Done                    ./pong 3
```