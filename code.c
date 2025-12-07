#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 3
#define DAILY_LIMIT 20000    // Max withdrawal per day
#define HISTORY_FILE "history.txt"

// ===== Structure for User Account =====
typedef struct {
    char name[20];
    int pin;
    float balance;
    float withdrawn_today;
} User;

// ===== ATM CASH STORAGE =====
int atm_2000 = 20;    // ?2000 notes
int atm_500  = 40;    // ?500 notes
int atm_100  = 100;   // ?100 notes

// ===== USERS DATABASE =====
User users[MAX_USERS] = {
    {"Arjun", 1111, 35000, 0},
    {"Megha", 2222, 18000, 0},
    {"Ravi",  3333, 12000, 0}
};

// ===== Save transaction to file =====
void save_history(char *name, char *action, float amount) {
    FILE *f = fopen(HISTORY_FILE, "a");
    if (!f) return;

    fprintf(f, "%s | %s | Amount: %.2f\n", name, action, amount);
    fclose(f);
}

// ===== Display ATM cash status =====
void show_atm_cash() {
    printf("\n=== ATM Cash Status ===\n");
    printf("?2000 notes: %d\n", atm_2000);
    printf("?500 notes:  %d\n", atm_500);
    printf("?100 notes:  %d\n", atm_100);
}

// ===== Attempt cash withdrawal using denominations =====
int dispense_cash(float amount) {
    int needed = (int)amount;
    int c2000 = needed / 2000;
    if (c2000 > atm_2000) c2000 = atm_2000;
    needed -= c2000 * 2000;

    int c500 = needed / 500;
    if (c500 > atm_500) c500 = atm_500;
    needed -= c500 * 500;

    int c100 = needed / 100;
    if (c100 > atm_100) c100 = atm_100;
    needed -= c100 * 100;

    if (needed != 0) {
        return 0; // ATM cannot dispense exact amount
    }

    // Deduct from ATM
    atm_2000 -= c2000;
    atm_500 -= c500;
    atm_100 -= c100;

    printf("\nDispensed:\n");
    if (c2000 > 0) printf("?2000 x %d\n", c2000);
    if (c500 > 0)  printf("?500 x %d\n", c500);
    if (c100 > 0)  printf("?100 x %d\n", c100);

    return 1;
}

int main() {
    int pin, choice, i;
    float amount;
    User *current_user = NULL;

    printf("===== ADVANCED ATM SIMULATION =====\n");

    // ==== PIN LOGIN ====
    printf("Enter PIN: ");
    scanf("%d", &pin);

    for (i = 0; i < MAX_USERS; i++) {
        if (users[i].pin == pin) {
            current_user = &users[i];
            break;
        }
    }

    if (!current_user) {
        printf("Invalid PIN! Access denied.\n");
        return 0;
    }

    printf("\nWelcome, %s!\n", current_user->name);

    // ===== MENU LOOP =====
    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Check Balance\n");
        printf("2. Withdraw Money\n");
        printf("3. Deposit Money\n");
        printf("4. View ATM Cash\n");
        printf("5. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nCurrent Balance: %.2f\n", current_user->balance);
                break;

            case 2:
                printf("\nEnter amount to withdraw: ");
                scanf("%f", &amount);

                if (amount <= 0) {
                    printf("Invalid amount!\n");
                } 
                else if (amount > current_user->balance) {
                    printf("Insufficient balance!\n");
                } 
                else if (current_user->withdrawn_today + amount > DAILY_LIMIT) {
                    printf("Daily limit exceeded! (Limit: ?%d)\n", DAILY_LIMIT);
                } 
                else if ((int)amount % 100 != 0) {
                    printf("Amount must be multiple of 100.\n");
                } 
                else {
                    if (dispense_cash(amount)) {
                        current_user->balance -= amount;
                        current_user->withdrawn_today += amount;
                        printf("\nPlease collect your cash.\n");
                        save_history(current_user->name, "Withdrawal", amount);
                    } else {
                        printf("ATM cannot dispense this amount due to note shortage.\n");
                    }
                }
                break;

            case 3:
                printf("\nEnter amount to deposit: ?");
                scanf("%f", &amount);

                if (amount <= 0) {
                    printf("Invalid amount!\n");
                } else {
                    current_user->balance += amount;
                    printf("Amount deposited successfully.\n");
                    save_history(current_user->name, "Deposit", amount);
                }
                break;

            case 4:
                show_atm_cash();
                break;

            case 5:
                printf("\nThank you, %s! Have a great day.\n", current_user->name);
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
