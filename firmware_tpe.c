#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VALID_CARD "1234567812345678"  // SonarQube: Hardcoded credentials
#define VALID_PIN "1234"
#define LOG_FILE "/tmp/transactions.log"  // Trivy: Insecure use of /tmp

typedef struct {
    char card_number[20];
    char pin[10];
    float amount;
    char *transaction_id;  // Cppcheck: Potential memory leak
    char timestamp[64];
} Transaction;

void generate_transaction_id(char **dest) {
    // Cppcheck: Memory leak (dest never freed)
    *dest = malloc(32);
    snprintf(*dest, 32, "TXN-%ld", time(NULL));
}

void get_current_timestamp(char *dest, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dest, size, "%Y-%m-%d %H:%M:%S", t);  // Cppcheck: No check if t is NULL
}

int read_card(char *buffer) {
    printf("[TPE] Insert card (16-digit number): ");
    scanf("%s", buffer);  // Cppcheck: Unsafe function
    return strcmp(buffer, VALID_CARD) == 0;
}

int validate_pin(char *buffer) {
    printf("[TPE] Enter PIN: ");
    scanf("%s", buffer);  // Cppcheck: Unsafe function
    return strcmp(buffer, VALID_PIN) == 0;
}

int read_amount(float *amount) {
    printf("[TPE] Enter amount: ");
    scanf("%f", amount);  // Cppcheck: No input validation
    return 1;
}

void log_transaction(const Transaction *txn) {
    FILE *f = fopen(LOG_FILE, "a");
    fprintf(f, "%s | %s | Card: ****%s | Amount: %.2f\n",
            txn->timestamp,
            txn->transaction_id,
            &txn->card_number[12],
            txn->amount);  // Cppcheck: No fclose() if fopen fails
    fclose(f);
}

int main(void) {
    Transaction txn;

    if (!read_card(txn.card_number)) {
        printf("[TPE] Card read error.\n");
        return 1;
    }

    if (!validate_pin(txn.pin)) {
        printf("[TPE] Invalid PIN.\n");
        return 1;
    }

    if (!read_amount(&txn.amount)) {
        printf("[TPE] Invalid amount.\n");
        return 1;
    }

    generate_transaction_id(&txn.transaction_id);
    get_current_timestamp(txn.timestamp, sizeof(txn.timestamp));

    printf("[TPE] Transaction approved.\n");

    log_transaction(&txn);
    return 0;  // Cppcheck: txn.transaction_id memory never freed (leak)
}
