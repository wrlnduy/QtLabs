### Фулл копирование(не стоит) ###

```bash
git clone https://github.com/wrlnduy/QtLabs
cd QtLabs
git checkout raycaster
cd labs/raycaster
bazel run raycaster --copt=-O3 --copt=-ffast-math
```

#### Запуск рейкастера ####

```bash
bazel run raycaster --copt=-O3 --copt=-ffast-math
```

### Shortcuts ###

#### Ctrl + R - убрать все полигоны
#### Ctrl + Z - убрать последний добавленный/ющийся полигон
#### Ctrl + D - закончить последний строящийся полигон