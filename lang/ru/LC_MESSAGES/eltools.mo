��          �   %   �      `  O   a     �     �  "   �           "     C     T     t     �     �     �     �     �  s   �     d     z     �     �     �     �  .   �       �       �  �      �  �  �   �	  &   <
  N   c
  S   �
  Q     =   X  '   �  F   �  $     &   *  4   Q  4   �  4   �  4   �  �   %  7   �  -     $   C  '   h  %   �     �  @   �  9     x  P  +  �  c  �           	                            
                                                                                      where power - engine power (default voltage=0.38, cos=0.9 ece=0.95, mode=ac3)
 Error reading the file
 Error! Illegal use of options
 Error! Missing value from option!
 Error! Zero impedance
 Incorrect combinations of modes
 Incorrect data!
 Incorrect voltage value - '%s'
 Invalid argument!
 Invalid request
 Invalid value for -i
 Invalid value for -r
 Invalid value for -u
 Invalid value for -x
 License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
Written by Igor 'sp_r00t' Tyukalov.
 Point '%s' not found
 Too few arguments
 Unknown error
 Unknown mode - '%s'
 Unknown option '%s'
 Usage %s %s
 Usage: %s [options] filename points...
Where:
 Wrong number of arguments
 m - mode. Takes on values:
    1 - single-phase mode;
    2 - two-phase mode;
    3 - three-phase mode.
u - voltage, V;
filename - network file;
points - points of calculation of short-circuit currents.
 where:
 -h or --help - print help
 -v or --version - print version
 -p - mode. With the option - single-phase, in the absence of - three-phase
 -d - type of current. With the option - DC, in the absence of - AC
 -i - amperage,
 -u - voltage,
 -r - resistance,
 -x - reactance,
  where:
 mode - mode. 'dc' for D.C.,  'ac' for single-phase alternating current, 'ac3' for three-phase alternating current;
 voltage - motor voltage;
 power - engine power;
 cos - power factor;
 ece - energy conversion efficiency,
or:
 Project-Id-Version: PACKAGE VERSION
Report-Msgid-Bugs-To: 
POT-Creation-Date: 2017-12-29 02:25+0300
PO-Revision-Date: 2017-12-04 02:19+0300
Last-Translator: igor <tyukalov@bk.ru>
Language-Team: Russian
Language: ru
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
  где power - мощность двигателя в кВт (по умолчанию напряжение 0.38 кВ, косинус - 0.9, КПД - 0.95,  mode - ac3)
 Ошибка чтения файла!
 Ошибка! Неправильное использование опции!
 Ошибка! Требуется указать значение для опции
 Ошибка! Расчётное сопротивление равно нулю!
 Неправильная комбинация режимов
 Некорректные данные!
 Некорректное значение напряжения - '%s'
 Некорректная опция
 Некорректный запрос
 Неправильное значение для -i
 Неправильное значение для -r
 Неправильное значение для -u
 Неправильное значение для -x
 Лицензия GPLv3+: CNU GPL версии 3 или последующей <http://gnu.org/licenses/gpl.html>.
Автор программы - Игорь 'sp_r00t' Тюкалов.
 Расчётная точка '%s' не найдена
 Слишком мало аргументов
 Неизвестная ошибка
 Неизвестная режим - %s
 Неизвестная опция %s
 Используйте: %s %s
 Используйте: %s [options] filename points...
Где:
 Неправильное число аргументов
 m - режим КЗ. Принимает следующие значения :
    1 - однофазный режим;
    2 - двухфазный режим;
    3 - трёхфазный режим.
u - напряжение, В;
filename - файл с описанием сети (смю Документацию);
points - точки для расчёта токов КЗ.
 где:
 -h or --help - показать справку
 -v or --version - показать версию
 -p -  режим. Однофазный в случае наличии опции и трёхфазный в случае отсутствия
 -d - род тока. Постоянный в случае наличия опции и переменный в случае отсутствия
 -i - сила тока,
 -u - напряжение,
 -r - активное сопротивление,
 -x - реактивное сопротивление,
  где:
 mode - режим. 'dc' для постоянного тока, 'ac' для переменного однофазного, 'ac3' для  трехфазного;
 voltage - напряжение двигателя, кВ;
 power - мощность двигателя, кВт;
 cos - коэффициент мощности;
 ece - КПД; 
 или:
 