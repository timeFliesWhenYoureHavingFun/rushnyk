/*******************************************************
   Démonstration de multiplexing
   upgrade this to 36 leds using pins 2-13 (6x6 matrix)
********************************************************/

// remplacement des #define par des tableaux pour une matrice 6x6 => 36 LEDs
const int ANODE_PINS[] = {2, 3, 4, 5, 6, 7};    // 6 anodes
const int CATHODE_PINS[] = {8, 9, 10, 11, 12, 13}; // 6 cathodes
const int NUM_ANODES = sizeof(ANODE_PINS) / sizeof(ANODE_PINS[0]);
const int NUM_CATHODES = sizeof(CATHODE_PINS) / sizeof(CATHODE_PINS[0]);
const int NUM_LEDS = NUM_ANODES * NUM_CATHODES;

const int nombre_de_motifs = NUM_LEDS;

int etat = 0; // numéro du motif actuellement affiché

unsigned long avant = 0; // plus récent changement de motif
const long intervalle = 150; // temps écoulé avant de changer de motif (ajustable)

void setup()
{
  // toutes les broches seront des sorties
  for (int i = 0; i < NUM_ANODES; ++i) pinMode(ANODE_PINS[i], OUTPUT);
  for (int i = 0; i < NUM_CATHODES; ++i) pinMode(CATHODE_PINS[i], OUTPUT);

  efface_tout();
}

void loop()
{
  // réglage de chacune des LEDs, une après l'autre
  for (int i = 1; i <= NUM_LEDS; i++)
  {
    if (motifOn(etat, i)) {
      reglage_led(i);
    }
    else {
      efface_tout(); // pour que l'intensité lumineuse ne dépende pas du nombre de LEDs allumées
    }
  }

  // on vérifie si le temps est venu de changer de motif:
  unsigned long maintenant = millis();

  if (maintenant - avant >= intervalle) {
    avant = maintenant;
    etat++;
    if (etat == nombre_de_motifs) {
      etat = 0;
    }
  }
}

// renvoie true si, pour l'état donné, la LED 'led' doit être allumée
bool motifOn(int state, int led) {
  // simple chase : un seul bit allumé à la fois
  return (state == (led - 1));
}

void efface_tout() // on met les broches dans l'état où tout est éteint
{
  // anodes LOW, cathodes HIGH pour éteindre toute la matrice
  for (int i = 0; i < NUM_ANODES; ++i) digitalWrite(ANODE_PINS[i], LOW);
  for (int i = 0; i < NUM_CATHODES; ++i) digitalWrite(CATHODE_PINS[i], HIGH);
}

void reglage_led(int numero)
{
  efface_tout(); // on éteint la LED précédente, s'il y a lieu

  int idx = numero - 1;
  int anodeIndex = idx % NUM_ANODES;
  int cathodeIndex = idx / NUM_ANODES;

  digitalWrite(ANODE_PINS[anodeIndex], HIGH);
  digitalWrite(CATHODE_PINS[cathodeIndex], LOW);
}
