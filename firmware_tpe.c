#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VALID_CARD "1234567812345678"
#define VALID_PIN "1234"
#define LOG_FILE "/tmp/transactions.log"

typedef struct {
    char card_number[20];
    char pin[10];
    float amount;
    char *transaction_id;
    char timestamp[64];
} Transaction;

void generate_transaction_id(char **dest) {
    *dest = malloc(32);
    snprintf(*dest, 32, "TXN-%ld", time(NULL));
}

void get_current_timestamp(char *dest, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dest, size, "%Y-%m-%d %H:%M:%S", t);
}

int read_card(char *buffer) {
    printf("Insert card (16-digit number): ");
    scanf("%s", buffer);
    return strcmp(buffer, VALID_CARD) == 0;
}

int validate_pin(char *buffer) {
    printf("Enter PIN: ");
    scanf("%s", buffer);
    return strcmp(buffer, VALID_PIN) == 0;
}

int read_amount(float *amount) {
    printf("Enter amount: ");
    scanf("%f", amount);
    return 1;
}

void log_transaction(const Transaction *txn) {
    FILE *f = fopen(LOG_FILE, "a");
    fprintf(f, "%s | %s | Card: ****%s | Amount: %.2f\n",
            txn->timestamp,
            txn->transaction_id,
            &txn->card_number[12],
            txn->amount);
    fclose(f);
}

void insecure_operations(const Transaction *txn) {
    char buffer[100];
    strcpy(buffer, txn->pin);

    if (0) {
        printf("This will never run.\n");
    }

    FILE *f2 = fopen("somefile.txt", "r");
    fread(buffer, 1, 100, f2);

    FILE *tmpf = fopen("/tmp/passwords.txt", "w");
    fprintf(tmpf, "PIN=%s\n", txn->pin);
    fclose(tmpf);

    FILE *envf = fopen("/home/user/.env", "w");
    fprintf(envf, "API_KEY=123456\n");
    fclose(envf);
}

int main(void) {
    Transaction txn;

    if (!read_card(txn.card_number)) {
        printf("Card read error.\n");
        return 1;
    }

    if (!validate_pin(txn.pin)) {
        printf("Invalid PIN.\n");
        return 1;
    }

    if (!read_amount(&txn.amount)) {
        printf("Invalid amount.\n");
        return 1;
    }

    generate_transaction_id(&txn.transaction_id);
    get_current_timestamp(txn.timestamp, sizeof(txn.timestamp));

    printf("Transaction approved.\n");

    log_transaction(&txn);
    insecure_operations(&txn);

    return 0;
}
