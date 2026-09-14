// --- GreenTracker Case v1.0 ---

/* [Dimensioni Generali Scatola] */
lunghezza = 95;  // Spazio per NodeMCU e Multiplexer
larghezza = 60;
altezza = 35;
spessore_muro = 2;

/* [Tolleranze e Smussi] */
tolleranza_incastro = 0.4; // Tolleranza per la stampa 3D del coperchio
raggio_angoli = 3;         // Arrotondamento degli angoli esterni
$fn = 60;                  // Risoluzione curve

/* [Display OLED 0.91"] */
oled_finestra_l = 25.5; // Area visibile dello schermo
oled_finestra_h = 7.5;
distanza_tra_oled = 16; // Distanza dal centro di un display all'altro

/* [Connessioni] */
usb_l = 13;
usb_h = 8;
foro_cavi_l = 15;
foro_cavi_h = 8;

// --- MODULI BASE ---

module scatola_arrotondata(l, w, h, r) {
    hull() {
        translate([-l/2+r, -w/2+r, 0]) cylinder(r=r, h=h);
        translate([ l/2-r, -w/2+r, 0]) cylinder(r=r, h=h);
        translate([-l/2+r,  w/2-r, 0]) cylinder(r=r, h=h);
        translate([ l/2-r,  w/2-r, 0]) cylinder(r=r, h=h);
    }
}

module case_base() {
    difference() {
        // Corpo esterno
        scatola_arrotondata(lunghezza, larghezza, altezza, raggio_angoli);
        
        // Scavo interno
        translate([0, 0, spessore_muro])
            scatola_arrotondata(lunghezza - spessore_muro*2, larghezza - spessore_muro*2, altezza, raggio_angoli);
            
        // Foro USB (centrato su un lato corto)
        translate([lunghezza/2, 0, spessore_muro + usb_h/2 + 2])
            cube([spessore_muro*4, usb_l, usb_h], center=true);
            
        // Foro uscita cavi sensore/i (lato opposto)
        translate([-lunghezza/2, 0, spessore_muro + foro_cavi_h/2 + 2])
            cube([spessore_muro*4, foro_cavi_l, foro_cavi_h], center=true);
    }
}

module coperchio() {
    difference() {
        union() {
            // Piano del coperchio
            scatola_arrotondata(lunghezza, larghezza, spessore_muro, raggio_angoli);
            
            // Labbro di incastro interno
            translate([0, 0, -spessore_muro])
                scatola_arrotondata(
                    lunghezza - spessore_muro*2 - tolleranza_incastro, 
                    larghezza - spessore_muro*2 - tolleranza_incastro, 
                    spessore_muro, 
                    raggio_angoli - 1
                );
        }
        
        // Scavo per alleggerire il labbro interno
        translate([0, 0, -spessore_muro - 0.1])
            scatola_arrotondata(
                lunghezza - spessore_muro*4 - tolleranza_incastro, 
                larghezza - spessore_muro*4 - tolleranza_incastro, 
                spessore_muro + 0.2, 
                raggio_angoli - 2
            );

        // Finestra OLED 1 (Superiore) - Posizione Z corretta per trapassare il tetto
        translate([0, distanza_tra_oled/2, 0])
            cube([oled_finestra_l, oled_finestra_h, 20], center=true);
            
        // Finestra OLED 2 (Inferiore) - Posizione Z corretta per trapassare il tetto
        translate([0, -distanza_tra_oled/2, 0])
            cube([oled_finestra_l, oled_finestra_h, 20], center=true);
    }
}

// --- ASSEMBLAGGIO CON COLORI ---

// Base della scatola in grigio scuro
color([0.25, 0.25, 0.28]) 
    case_base();

// Coperchio in verde (sollevato di 20mm in vista esplosa)
color([0.2, 0.7, 0.3]) 
    translate([0, 0, altezza + 20]) 
        coperchio();