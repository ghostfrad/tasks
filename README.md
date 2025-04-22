# FlightCodesComparator
Первое задание: cравнение двух строк содержащие код авиакомпании и номер рейса.

## Сборка (пример)
g++ -Wall -Wextra -o flight_compare FlightCodesComparator/main.cpp

## Запуск (пример)
./flight_compare "AFL1" "AFL0001"

В случаи равенства строк программа выведет "equal", в случаи неравенства строк или в случаи несоотвествия формата строки программа выведет "not equal".

# FlightCodesDeduplicator
Второе задание: параллельная обработка двух файлов с исходными данными (1_in.txt, 2_in.txt) с целю отбросить
неуникальные комбинации авиакомпании и номера рейса. Результат сохраняется в
файлы 1_out.txt и 2_out.txt соотвественно.

## Сборка (пример)
g++ -Wall -Wextra -pthread -o flight_dedublicator FlightCodesDeduplicator/main.cpp

## Запуск (пример)
./flight_dedublicator

В текущей папке должны быть исходные текстовые файлы (1_in.txt, 2_in.txt)
