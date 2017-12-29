Пакет предоставляет небольшие консольные программы для
электротехнических расчётов:
		   om       - для расчётов по закону Ома,
		   motor  - для расчёта тока электродвигателя,
		   vl380   - для расчёта падений напряжений в кабельных трёхфазных сетях 0.4 кВ,
		   scc       - для расчётов токов КЗ в радиальных сетях
Так же представлена библиотека libnet предоставляющая представление электрических сетей
(на данный момент только радиальных) и некоторые функции для работы с таким
представлением.

Пакет позиционируется как кроссплатформенный (*nix/win). Тестировался на Debian и
Windows XP. Сборка в Win требует наличия GNU make. Тестировалось при помощи данной
утилиты из пакета GNUWin. Однако сборка под Win проработана лишь частично.

Локальная сборка в *nix осуществляется командой
make
Инсталляция/деинсталляция на данный момент не предусмотрена, однако это будет
исправлено.
