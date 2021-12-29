# YouTube subscriber counter
Omar Čelik & Benjamin Emirhafizović\
Elektrotehnički fakultet Sarajevo\
Ugradbeni sistemi

Projekat YouTube subscriber counter je realizovan u mbed simulatoru. Prikaz podataka o kanalima vrši se na ST7789H2 displeju unutar simulatora.
U svrhu prikupljanja podataka o YouTube kanalima korišten je YouTube Data API, dostupan za upotrebu u nekomercijalne svrhe. Putem API moguće je pristupiti osnovnim statističkim podacima YouTube kanala kao što su broj pretplatnika, broj ukupnih pregleda, broj uploadovanih videa, država porijekla kanala, datum registracije itd.

Funkciju prikupljanja podataka vrši Python skripta, koja podatke o traženim kanalima prikuplja koristeći YouTube API i proslijeđuje ih na mbed simulator putem MQTT brokera i odgovarajućih tema koje prati kôd unutar simulatora. Kanali, koje je moguće unositi putem MQTTlens aplikacije, smještaju se unutar mbed koda u simulatoru.

Osim ST7789H2 displeja, koriste se tri tastera. Korisnik može listati unesene kanale koristeći se tasterima 1 i 3 (sljedeći i prethodni kanal) te osvježiti podatke o trenutnom kanalu u bilo kojem momentu koristeći taster 2. Svaka promjena kanala ažurira njegove podatke.


# Demonstrativni video
https://www.youtube.com/watch?v=rCIkcuch-Oo
