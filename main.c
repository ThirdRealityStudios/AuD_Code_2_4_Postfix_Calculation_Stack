#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Knoten
{
    // Wert des Knotens.
    int key;

    // Nachfolge-Knoten.
    struct Knoten *next;
};

// "z" ist das Ende vom Stack (der Trick bei "z" ist,
// dass man später beim Runternehmen keine Sonderfälle beachten muss,
// da es auf sich selber zeigt).
// "t" ist nur ein temporärer Knoten, wenn ein neuer erzeugt werden soll.
// "Kopf" ist der Anfang vom Stack.
struct Knoten *z, *t, *Kopf;

/**
  * Den Stapel initialisieren mit den Standardelementen
  * "Kopf" (Anfangselement ganz oben auf'm Stapel) und
  * "z" (Endelement ganz unten, das lediglich das Ende markiert).
  */
void stackinit()
{
    // Hier wird ein Kopf-Element erstellt,
    // das jedoch nicht für die Datenhaltung gedacht ist.
    // Es soll nur den Anfang des Stacks markieren.
	Kopf = (struct Knoten *) calloc (1, sizeof *Kopf);

    // Hier ein weiterer Knoten erzeugt,
    // der ebenfalls nicht für die Datenhaltung gedacht ist.
    // Er dient lediglich der Markierung des Endes vom Stack.
	z = (struct Knoten *) calloc (1, sizeof *z);

    // Das Kopf zeigt jetzt direkt auf das Ende vom Stack.
    // Der Stack soll also leer sein.
	Kopf->next = z;

    // Das Ende zeigt auf sich selber.
	z->next = z;
}

void push (int v)
{
    // Den neuen Knoten erzegen als "t" für "temporär".
	t = (struct Knoten *) malloc(sizeof *t);

    // Den Knoten mit dem entsprechenden Wert initialisieren.
	t->key = v;

    /*
     * Der Kopf ist beim Stack in dieser Implementierung
     * bildlich gesehen oben auf dem Stapel.
     * Das Ende ist somit immer unten.
     *
     * Was hier also geschieht ist Folgendes:
     *
     * Das neue Stapel-Element zeigt im ersten Schritt
     * hier auf das vorherige oberste Element des Stapels (Kopf -> next).
     */
    t -> next = Kopf -> next;

    /*
     * Der vorherige Schritt bringt aber noch nicht viel,
     * da der Anfang vom Stapel ("oben" also in der Implementierung bildlich gesehen)
     * dieses neue Stapel-Element noch nicht kennt.
     * Es ist bisher so,
     * dass das neue Stapel-Element
     * nur schon seinen Nachfolger "unten drunter" (vorheriges oberstes Stapel-Element).
     *
     * Daher passiert in der Wertzuweisung hierunter nun Folgendes:
     * der Stapel-Anfang (Kopf -> next) soll jetzt auf das neue
     * oberste Stapel-Element zeigen (t also).
     */
	Kopf -> next = t;

    // Somit wurde bis hier nun ein neues Stapel-Element auf den Stack gelegt..
}

int pop()
{
	int x;

    // Das oberste Stapel-Element (oben drauf also) als temporären
    // Knoten t merken,
    // damit dieser runtergenommen wird.
	t = Kopf -> next;

    // Der Stapel-Anfang soll jetzt einfach auf den Nachfolger
    // vom obersten Stapel-Element zeigen (das Element unter dem obersten Element also).
    // Zur Erinnerung: der Kopf bzw. Anfang ist in dieser Implmentierung ja ganz oben.
    Kopf -> next = t -> next;

    // Den Wert merken vom runtergenommenen Stapel-Element,
    // bevor dieses Element mit free(..) im Arbeitsspeicher freigegeben wird (da ja nicht mehr benötigt).
	x = t -> key;

    // TODO: free(..) nicht ausführen, wenn der Knoten t auf sich selber zeigt (= dann offensichtlich das Stapelende)
    // Den Speicherbereich des runtergenommenen Stapel-Elements im Arbeitsspeicher freigeben.
	if(t != z)
        free(t);

    // Den Wert vom runtergenommenen Stapel-Element zurückgeben.
	return x;
}

void test_stackinit()
{
    puts("Testing stackinit(..)");
    
    stackinit();

    assert(Kopf -> key == 0);
    puts("OK");

    assert(z -> key == 0);
    puts("OK");

    assert(Kopf -> next == z);
    puts("OK");

    assert(z -> next = z);
    puts("OK");

    free(Kopf);
    Kopf = NULL;

    free(z);
    z = NULL;
}

void test_push()
{
    struct Knoten *el1 = NULL, *el2 = NULL, *el3 = NULL;

    puts("Testing push(..)");

    stackinit();

    assert(Kopf -> next == z); // size 0
    puts("OK");

    push(0);
    el1 = t; // Remember first pushed element.
    assert(Kopf -> next == el1);
    assert(el1 -> next == z);
    assert(z -> next == z);
    assert(Kopf -> next != z);
    assert(el1 -> key == 0);
    puts("OK"); // size must be 1 now

    push(-12);
    el2 = t;
    assert(Kopf -> next == el2);
    assert(el2 -> next == el1);
    assert(el1 -> next == z);
    assert(z -> next == z);
    assert(Kopf -> next != z);
    assert(el1 -> key == 0);
    assert(el2 -> key == -12);
    puts("OK"); // size 2

    push(73);
    el3 = t;
    assert(Kopf -> next == el3);
    assert(el3 -> next == el2);
    assert(el2 -> next == el1);
    assert(el1 -> next == z);
    assert(z -> next == z);
    assert(Kopf -> next != z);
    assert(el1 -> key == 0);
    assert(el2 -> key == -12);
    assert(el3 -> key == 73);
    puts("OK"); // size 3

    free(Kopf);
    Kopf = NULL;

    free(el1);
    el1 = NULL;

    free(el2);
    el2 = NULL;

    free(el3);
    el3 = NULL;

    free(z);
    z = NULL;
}

void test_pop()
{
    int x = 0;

    puts("Testing pop(..)");

    stackinit();

    x = pop();

    assert(Kopf != z);
    assert(Kopf -> next == z);
    assert(x == 0);
    puts("OK");

    free(Kopf);
    Kopf = NULL;

    free(z);
    z = NULL;
}

void unitTest()
{
    puts("Unit-Testing:");
    puts("");

    test_stackinit();
    test_push();
    test_pop();

    puts("Finished all unit");

    puts("");
}

void main(void) // 5+2 = Infix, Postfix = 52*, Präorder: +52
{
    unitTest();

    puts("Starting postorder calculation programm..");

    // Repräsentiert jedes eingelene Zeichen per getchar().
	char c;

    // Zur Speicherung der Operanden oder des Endergebnis'.
    int x;

    // Den Stack initialisieren,
    // dann ein oder mehrere Zeichen einlesen in den Inputstream per getchar()
    // und das erste Zeichen aus dem Inputstream muss != '.' sein,
    // da sonst das Programm abgebrochen wird.
	for(stackinit(); (c = getchar()) != '.'; )
	{
		x = 0;

		if (c == '+')
        {
            puts("Addition..");
            x = pop() + pop(); // Direkt zwei Operanden vom Stapel nehmen und addieren.
        }

		if (c == '*')
        {
            puts("Mul..");
            x = pop() * pop(); // Gleiches hier, aber mit Multiplikation
        }

        /*
         * Überprüfen,
         * ob man eine Dezimalzahl eingegeben hat.
         * Dies geschieht hier Ziffer für Ziffer.
         * Es wird hier jedes Ziffernzeichen in ein Integer umgewandelt.
         * Dabei wird das Horner-Schema angewendet,
         * um den korrekten Stellenwert und folglich die eingegebene Zahl zu errechnen
         */
		while(c >= '0' && c <= '9')
		{
			x = x * 10 + (c - '0'); // Ziffer in rechenbaren Integer umwandeln und Horner-Schema anwenden.

            puts("Reading further digits..");
            c = getchar(); // Neuen Stellenwert einlesen.

            if(c == '\n')
                puts("Skipping line-break symbol..");
		}

        puts("Pushing operand..");

        /*
         * Verständnisfrage zu dieser if-Bedingung:
         *
         * Warum soll x hier ungleich 0 sein?
         * Was ist, wenn ich 0*1 rechne?
         * Dann kann ich das Ergebnis nicht anzeigen lassen bzw. noch nicht mal ausrechnen lassen?
         */
		if(x != 0)
        {
            /*
             * Anmerkung zur push-Operation:
             * 
             * Hier werden nicht die eingelesenen Zeichenwerte auf den Stack gepusht,
             * sondern die umgewandelten Gegenstücke zu ihnen,
             * sprich die Dezimalzahlen,
             * die sie repräsentieren (= der arithmetische Integer-Wert, der repräsentiert wird durch die Zeichen).
             * 
             * Außerdem hat das Line-Break jeder getchar()-Eingabebestätigung keinen Einfluss
             * auf den gepushten Wert hier (hier wird ja x gepusht und nicht c, also das eingelsene Zeichen).
             */

            push(x); // Einen Operanden oder das Endergebnis (je nach Situation) auf den Stack pushen.
        }
	}

    puts("Printing result..");

    // Das Ergebnis auf der Konsole ausgeben, indem man es vom Stack holt.
	printf ("%d\n", pop());
}
