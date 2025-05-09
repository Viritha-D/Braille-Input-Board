#define BUTTON1 5
#define BUTTON2 6
#define BUTTON3 7
#define BUTTON4 8
#define BUTTON5 9
#define BUTTON6 10

#define sizeH 100
#define MY_delay 50 


typedef struct pat {
    char* pattern;
    char chr;
    struct pat* next;
} pat;

typedef struct {
    pat* table[sizeH];
} HashM;


HashM Bmap;


unsigned int hash(const char* pattern);
void init_(HashM* Bmap);
void insert(HashM* Bmap, const char* pattern, char chr);
char get(HashM* Bmap, const char* pattern);
void USART_Init();
void USART_Transmit(char data);
void USART_PrintString(const char* str);
void read_buttons(char* pattern);
void record_button_pattern(char* pattern, unsigned long duration);


unsigned int hash(const char* pattern) {
    unsigned int h = 0;
    while (*pattern) {
        h = (h << 1) | (*pattern - '0');
        pattern++;
    }
    return h % sizeH;
}


void init_(HashM* Bmap) {
    for (int i = 0; i < sizeH; i++) {
        Bmap->table[i] = NULL;
    }
}


void insert(HashM* Bmap, const char* pattern, char chr) {
    unsigned int index = hash(pattern);
    
    pat* newI = (pat*)malloc(sizeof(pat));
    newI->pattern = strdup(pattern);
    newI->chr = chr;
    newI->next = NULL;

    if (Bmap->table[index] == NULL) {
        Bmap->table[index] = newI;
    } else {
        pat* current = Bmap->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newI;  
    }
}


char get(HashM* Bmap, const char* pattern) {
    unsigned int index = hash(pattern);
    pat* current = Bmap->table[index];

    while (current != NULL) {
        if (strcmp(current->pattern, pattern) == 0) {
            return current->chr; 
        }
        current = current->next;
    }
    return '\0';
}

void USART_Init() {
    Serial.begin(9600);
}


void USART_Transmit(char data) {
    Serial.write(data);
}


void USART_PrintString(const char* str) {
    while (*str) {
        USART_Transmit(*str++);
    }
}

void record_button_pattern(char* pattern, unsigned long duration) {
    USART_PrintString("Counter started");
    USART_PrintString("\n");
    unsigned long startTime = millis();
    memset(pattern, '0', 6);  
    pattern[6] = '\0';         

    while (millis() - startTime < duration) { 
        if (digitalRead(BUTTON1) == LOW) {
            pattern[0] = '1'; 
        }
        if (digitalRead(BUTTON2) == LOW) {
            pattern[1] = '1'; 
        }
        if (digitalRead(BUTTON3) == LOW) {
            pattern[2] = '1'; 
        }
        if (digitalRead(BUTTON4) == LOW) {
            pattern[3] = '1';
        }
        if (digitalRead(BUTTON5) == LOW) {
            pattern[4] = '1'; 
        }
        if (digitalRead(BUTTON6) == LOW) {
            pattern[5] = '1'; 
        }

  
        delay(10);
    }
}

void setup() {
    USART_Init();

    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(BUTTON3, INPUT);
    pinMode(BUTTON4, INPUT);
    pinMode(BUTTON5, INPUT);
    pinMode(BUTTON6, INPUT);

    init_(&Bmap);
    insert(&Bmap, "100000", 'A'); 
    insert(&Bmap, "101000", 'B'); 
    insert(&Bmap, "110000", 'C'); 
    insert(&Bmap, "110100", 'D'); 
    insert(&Bmap, "100100", 'E'); 
    insert(&Bmap, "111000", 'F'); 
    insert(&Bmap, "111100", 'G');
    insert(&Bmap, "101100", 'H');
    insert(&Bmap, "011000", 'I');
    insert(&Bmap, "011100", 'J');
    insert(&Bmap, "100010", 'K');
    insert(&Bmap, "101010", 'L');
    insert(&Bmap, "110010", 'M');
    insert(&Bmap, "110110", 'N');
    insert(&Bmap, "100110", 'O');
    insert(&Bmap, "111010", 'P');
    insert(&Bmap, "111110", 'Q');
    insert(&Bmap, "101110", 'R');
    insert(&Bmap, "011010", 'S');
    insert(&Bmap, "011110", 'T');
    insert(&Bmap, "100011", 'U');
    insert(&Bmap, "101011", 'V');
    insert(&Bmap, "011101", 'W');
    insert(&Bmap, "110011", 'X');
    insert(&Bmap, "110111", 'Y');
    insert(&Bmap, "100111", 'Z');
    insert(&Bmap, "010111", '#');
    insert(&Bmap, "001101", '.');
    insert(&Bmap, "001000", ',');
    
  
}

void loop() {
    char patt[7];
  
    record_button_pattern(patt, 6000); 

    char chr = get(&Bmap, patt);
    if (chr != '\0') {
        USART_PrintString("Character found: ");
        USART_Transmit(chr);
        USART_PrintString("\n");
    }

    delay(50);
}