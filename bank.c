#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store account information
struct Account
{
    char name[50];
    char dob[15];
    char phone[15];
    char password[20];
    float balance;
};

// Function to create a new account
void createAccount()
{
    struct Account acc;
    FILE *fp = fopen("accounts.txt", "a+");  // Open the file in append and read mode
    if (!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    // Check if the phone number already exists
    char phone[15];
    int exists = 0;
    printf("Enter Phone Number (as Account Number): ");
    scanf("%s", phone);

    // Check for duplicate phone number
    struct Account tempAcc;
    while (fread(&tempAcc, sizeof(struct Account), 1, fp))
    {
        if (strcmp(tempAcc.phone, phone) == 0)
        {
            exists = 1;
            break;
        }
    }

    if (exists)
    {
        printf("Error: This phone number already has an account.\n");
        fclose(fp);
        return;
    }

    // If no duplicate, proceed to create the account
    acc.balance = 0.0;
    strcpy(acc.phone, phone);  // Use the phone number entered

    getchar();  // Clear newline from buffer
    printf("Enter Full Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = 0;  // Remove newline character

    printf("Enter Date of Birth (DD-MM-YYYY): ");
    scanf("%s", acc.dob);

    printf("Set Password: ");
    scanf("%s", acc.password);

    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!\n");
}

// Function to check balance
void checkBalance()
{
    char phone[15], password[20];
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%s", phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (strcmp(acc.phone, phone) == 0 && strcmp(acc.password, password) == 0)
        {
            printf("Current Balance: $%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("Account not found or incorrect password.\n");
    }
}

// Function to deposit money
void deposit()
{
    char phone[15], password[20];
    float amount;
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%s", phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r+");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (strcmp(acc.phone, phone) == 0 && strcmp(acc.password, password) == 0)
        {
            printf("Enter Amount to Deposit: $");
            scanf("%f", &amount);
            acc.balance += amount;

            fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
            fwrite(&acc, sizeof(struct Account), 1, fp);
            found = 1;
            printf("Deposit Successful!\n");
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("Account not found or incorrect password.\n");
    }
}

// Function to withdraw money
void withdraw()
{
    char phone[15], password[20];
    float amount;
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%s", phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r+");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (strcmp(acc.phone, phone) == 0 && strcmp(acc.password, password) == 0)
        {
            printf("Enter Amount to Withdraw: $");
            scanf("%f", &amount);

            if (amount > acc.balance)
            {
                printf("Insufficient Balance!\n");
            }
            else
            {
                acc.balance -= amount;
                fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
                fwrite(&acc, sizeof(struct Account), 1, fp);
                printf("Withdrawal Successful!\n");
            }
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("Account not found or incorrect password.\n");
    }
}
// Function to transfer balance
void transferBalance()
{
    char senderPhone[15], senderPassword[20], receiverPhone[15];
    float amount;
    struct Account sender, receiver;
    int senderFound = 0, receiverFound = 0;

    printf("Enter Your Phone Number: ");
    scanf("%s", senderPhone);
    printf("Enter Your Password: ");
    scanf("%s", senderPassword);

    printf("Enter Receiver's Phone Number: ");
    scanf("%s", receiverPhone);

    FILE *fp = fopen("accounts.txt", "r+");

    // Check if sender account exists and has sufficient balance
    while (fread(&sender, sizeof(struct Account), 1, fp))
    {
        if (strcmp(sender.phone, senderPhone) == 0 && strcmp(sender.password, senderPassword) == 0)
        {
            printf("Enter Amount to Transfer: $");
            scanf("%f", &amount);

            if (amount > sender.balance)
            {
                printf("Insufficient Balance!\n");
                fclose(fp);
                return;
            }

            sender.balance -= amount;
            senderFound = 1;
            fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
            fwrite(&sender, sizeof(struct Account), 1, fp);
            break;
        }
    }

    rewind(fp);

    // Check if receiver account exists
    while (fread(&receiver, sizeof(struct Account), 1, fp))
    {
        if (strcmp(receiver.phone, receiverPhone) == 0)
        {
            receiver.balance += amount;
            receiverFound = 1;
            fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
            fwrite(&receiver, sizeof(struct Account), 1, fp);
            break;
        }
    }
    fclose(fp);

    if (senderFound && receiverFound)
    {
        printf("Transfer Successful!\n");
    }
    else
    {
        printf("Transfer Failed! Check account details.\n");
    }
}

// Function to view account status
void accountStatus()
{
    char phone[15], password[20];
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%s", phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (strcmp(acc.phone, phone) == 0 && strcmp(acc.password, password) == 0)
        {
            printf("\n=== Account Status ===\n");
            printf("Name: %s\n", acc.name);
            printf("Account Number: %s\n", acc.phone);
            printf("Current Balance: $%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("Account not found or incorrect password.\n");
    }
}
// Main Menu
int main()
{
    int choice;
    do
    {
        printf("\n=== Bank Management System ===\n");
        printf("1. Create Account\n");
        printf("2. Check Balance\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Transfer Balance\n");
        printf("6. Account Status\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            checkBalance();
            break;
        case 3:
            deposit();
            break;
        case 4:
            withdraw();
            break;
        case 5:
            transferBalance();
            break;
        case 6:
            accountStatus();
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
