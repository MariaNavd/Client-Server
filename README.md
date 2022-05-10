# Client-Server

Приложение состоит из двух частей — серверной **Server**: [server.cpp](https://github.com/MariaNavd/Client-Server/blob/master/Server/server.cpp), [main.cpp](https://github.com/MariaNavd/Client-Server/blob/master/Server/main.cpp) и клиентской **Client**: [client.cpp](https://github.com/MariaNavd/Client-Server/blob/master/Client/client.cpp), [main.cpp](https://github.com/MariaNavd/Client-Server/blob/master/Client/main.cpp). Взаимодействие между процессами осуществляется с помощью разделяемой памяти.

### Функционал сервера
* Создание средства взаимодействия — разделяемой памяти. Доступ к памяти (чтение/запись) регулируется семафорами.
* Ведение журнала событий **journal.log**.
* Обработка запросов клиента — взаимодействие с "базой данных" сотрудников **Data**: [data.cpp](https://github.com/MariaNavd/Client-Server/blob/master/Server/Data/data.cpp).
  
  Сервер может обрабатывать следующие запросы:
  
  * поиск сотрудника по ID
    ```
    ID [id_val]
    
    в случае успешного поиска возвращается имя сотрудника, если сотрудник не найден, возвращается `Cannot find requested data`
    ```
  * поиск сотрудника по имени
    ```
    NAME [name]

    в случае успешного поиска возвращается ID сотрудника, если сотрудник не найден, возвращается `Cannot find requested data`
    ```
  * добавление сотрудника в базу
    ```
    ADD [name]

    возвращается ID, присвоенное добавленному сотруднику
    ```

  При получении кодового сообщения `QUIT_SERVER` сервер уничтожает средство взаимодействия и останавливает работу. В случае превышения времени ожидания сообщения от клиента сервер также прекращает свою работу.

### Функционал клиента
* Подключение к разделяемой памяти по ключу.
* Передача сообщений серверу. В случае остановки сервера клиент завершает свою работу.

## Компиляция 
* Linux: требуется наличие CMake на системе 
  ```
  python build.py 
  ```

## Запуск 
Запуск сервера:
```
./server [--timeout $TIMEOUT_VAL] 

дополнительные параметры:
--timeout $TIMEOUT_VAL    максимальное время ожидания сообщения от клиента (сек)
```
После запуска сервера запускается клиент:
```
./client [--with-sleep $SLEEP_TIME_VAL] 

дополнительные параметры:
--with-sleep $SLEEP_TIME_VAL    время в течение которого клиент не отправляет сообщения серверу, но продолжает работу (сек)
```

#### Пример работы
* Остановка сервера по таймауту
  ```
  ./server --timeout 15                                 ./client --with-sleep 20

  Server started
  Server is ready to read                               Asking server ID 237620
  READ: ID 237620
  Search person by ID                                   
  ANSW: Elmer Page                                      ANSW: Elmer Page

  Server is ready to read                               Asking server ADD Helen Collins
  READ: ADD Helen Collins
  Add a new person Helen Collins
  ANSW: A new person was added with ID 909150           ANSW: A new person was added with ID 909150

  Server is ready to read                               Asking server ID 568742
  READ: ID 568742
  Search person by ID
  ANSW: Cannot find requested data                      ANSW: Cannot find requested data
          
  Server is ready to read                               /* Sleep 20 sec */
  TIMEOUT
  Server stopped by timeout                             Server stopped by timeout
  ```
  journal.log
  ```
  Tue May 10 14:54:55 2022
	  Server started
  Tue May 10 14:54:56 2022
	  Search person by ID
  Tue May 10 14:54:56 2022
	  Add a new person Helen Collins
  Tue May 10 14:54:56 2022
	  Search person by ID
  Tue May 10 14:55:11 2022
	  Server stopped by timeout
  ```
* Остановка сервера по кодовому сообщению
  ```
  ./server --timeout 45                                 ./client --with-sleep 5
  
  Server started
  Server is ready to read                               Asking server ID 237620 
  READ: ID 237620
  Search person by ID
  ANSW: Elmer Page                                      ANSW: Elmer Page

  Server is ready to read                               Asking server ADD Helen Collins 
  READ: ADD Helen Collins
  Add a new person Helen Collins
  ANSW: A new person was added with ID 808894           ANSW: A new person was added with ID 808894

  Server is ready to read                               Asking server ID 568742
  READ: ID 568742
  Search person by ID
  ANSW: Cannot find requested data                      ANSW: Cannot find requested data

                                                        /* Sleep 5 sec */
  Server is ready to read                               Asking server NAME Michael Hunt
  READ: NAME Michael Hunt
  Search person by NAME
  ANSW: 925847                                          ANSW: 925847

  Server is ready to read                               Asking server NAME James Lane
  READ: NAME James Lane
  Search person by NAME
  ANSW: Cannot find requested data                      ANSW: Cannot find requested data

  Server is ready to read                               Asking server ADD James Lane
  READ: ADD James Lane
  Add a new person James Lane
  ANSW: A new person was added with ID 715436           ANSW: A new person was added with ID 715436

  Server is ready to read                               Asking server QUIT_SERVER
  READ: QUIT_SERVER
  Server stopped                                        ANSW: Server stopped
  ```
  journal.log
  ```
  Tue May 10 15:02:40 2022
  	Server started
  Tue May 10 15:02:41 2022
	  Search person by ID
  Tue May 10 15:02:41 2022
	  Add a new person Helen Collins
  Tue May 10 15:02:41 2022
	  Search person by ID
  Tue May 10 15:02:46 2022
	  Search person by NAME
  Tue May 10 15:02:46 2022
	  Search person by NAME
  Tue May 10 15:02:46 2022
	  Add a new person James Lane
  Tue May 10 15:02:46 2022
	  Server stopped
  ```
