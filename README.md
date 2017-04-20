# bachelorprosjekt

SLAVE-PROGRAMMET:
Inni bachelorprosjekt\Keil\Master og Slave\examples\ble_peripheral_SLAVE\ble_app_template_1\pca10040\s132\arm5_no_packs
ligger koden som slaven kjører. Under Applications hvor main.c er, ligger ble_detect.c. Denne filen setter opp en service som heter 
ble_detect med en egen service UUID. Denne servicen inneholder for tiden èn characteristics med egen charachteristisk UUID (se ble_detect.h). 
Denne characteristicen tar imot verdien til sensoren og sender denne til softdevicen og verdien kan sees hvis man kobler til med bluetooth
(enheten heter NRF_Slave). funksjonen sensor_characteristic_update() i bunnen av ble_detect.c oppdaterer characteristicsen med verdier fra 
funksjonen door_is_closed() i main. Dette er en midlertidig funksjon som incrementer en variabel "temperature" hver gang funksjonen
kjøres. Dette bestemmes ut ifra en timer:

        APP_TIMER_DEF(m_our_door_timer_id);
        #define TOGGLE_DOOR_TIMER    APP_TIMER_TICKS(10000, APP_TIMER_PRESCALER) // Interval         for closing the door. 10 seconds.
        
        timeren opprettes i timers_init(void);
        og startes i application_timers_start();
     
Den egentlige funksjonen som skal brukes med sensor heter gpio_init(), denne sjekker om signalet på pinne 9 er høyt eller lavt,
og oppdaterer characteristics med dette. Tanken her er å starte en timer som varer like lenge som det tar å lukke porten, og hvis det ikke
er noe hindring på dette intervallet skal chipen gå i sovemodus. Med andre ord er porten nå lukket. En annen ide er å ha denne timeren i
masteren, og at den oppdaterer en ny characteristics når timeren er utløpt som forteller slaven at den kan sove. 

Hvis den oppdager en hindring oppdateres dette i characteristics og masteren vil åpne porten. Slaven kan gå i sovemodus etter dette. Lukking
misslyktes. 

MASTER-programmet:
Tar utganspunkt i bachelorprosjekt\Keil\Master og Slave\examples\ble_central_MASTER\ble_app_hrs_c\pca10040\s132\arm5_no_packs.
Her må det lages en ny service(?) som nå heter ble_detect_c. Denne skal behandle dataene som slaven sender ut og bruke disse til å
skjønne om det er noen hindring eller ikke. 
Det må også lages en ny service(?) eller en ny characteristics inni den servicen vi har fra før av, som oppdaterer en variabel på at porten
skal lukkes. Det er når slaven merker at denne variablen endres at den skal våkne og kjøre gpio_init()
Det må også være en funsksjon i main som senser på en pinne om at den får "lukke-signal". Den skal da opptdatere det som står over. 
Den må også ha en funksjon som brukes til output pin for å sende signal til garasjeportåpneren. 

        
