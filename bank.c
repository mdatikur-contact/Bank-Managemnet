#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store account information
struct Account
{
    char name[50];
    char dob[15];
    int phone;
    char password[20];
    float balance;
};

// Function to create a new account
// Function to create a new account
void createAccount()
{
    struct Account acc;
    FILE *fp = fopen("accounts.txt", "a+"); // Open the file in append and read mode
    if (!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    // Check if the phone number already exists
    int phone;
    int exists = 0;
    printf("Enter Phone Number (as Account Number): ");
    scanf("%d", &phone);

    // Check for duplicate phone number
    struct Account tempAcc;
    while (fread(&tempAcc, sizeof(struct Account), 1, fp))
    {
        if (tempAcc.phone == phone)
        {
            exists = 1;
            break;
        }
    }

    if (exists)
    {
        printf("\n **Error: This phone number already has an account.**\n");
        fclose(fp);
        return;
    }

    // If no duplicate, proceed to create the account
    acc.balance = 0.0;
    acc.phone = phone; // Use the phone number entered

    getchar(); // Clear newline from buffer
    printf("Enter Full Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = 0; // Remove newline character

    printf("Enter Date of Birth (DD-MM-YYYY): ");
    scanf("%s", acc.dob);

    // Password validation
    while (1)
    {
        printf("Set Password (4 to 8 characters): ");
        scanf("%s", acc.password);
        int len = strlen(acc.password);
        if (len >= 4 && len <= 8)
        {
            break;
        }
        else
        {
            printf("\n**Error: Password must be between 4 and 8 characters.**\n");
        }
    }

    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!\n");
}

// Function to check balance
void checkBalance()
{
    int phone;
    char password[20];
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%d", &phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (acc.phone == phone && strcmp(acc.password, password) == 0)
        {
            printf("\n=== Balance Status ===\n");
            printf("Current Balance: $%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("\n **Account not found or incorrect password.**\n");
    }
}

// Function to deposit money
void deposit()
{
    int phone;
    char password[20];
    float amount;
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%d", &phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r+");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (acc.phone == phone && strcmp(acc.password, password) == 0)
        {
            printf("Enter Amount to Deposit: $");
            scanf("%f", &amount);
            acc.balance += amount;

            fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
            fwrite(&acc, sizeof(struct Account), 1, fp);
            found = 1;

            printf("\n == Deposit Successful! ==\n");
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("\n **Account not found or incorrect password.**\n");
    }
}

// Function to withdraw money
void withdraw()
{
    int phone;
    char password[20];
    float amount;
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%d", &phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r+");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (acc.phone == phone && strcmp(acc.password, password) == 0)
        {
            printf("Enter Amount to Withdraw: $");
            scanf("%f", &amount);

            if (amount > acc.balance)
            {
                printf("\n **Insufficient Balance!**\n");
            }
            else
            {
                acc.balance -= amount;
                fseek(fp, -(long)sizeof(struct Account), SEEK_CUR);
                fwrite(&acc, sizeof(struct Account), 1, fp);
                printf("\n ==Withdrawal Successful!==\n");
            }
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("\n**Account not found or incorrect password.**\n");
    }
}

// Function to transfer balance
void transferBalance()
{
    int senderPhone, receiverPhone;
    char senderPassword[20];
    float amount;
    struct Account sender, receiver;
    int senderFound = 0, receiverFound = 0;

    printf("Enter Your Phone Number: ");
    scanf("%d", &senderPhone);
    printf("Enter Your Password: ");
    scanf("%s", senderPassword);

    printf("Enter Receiver's Phone Number: ");
    scanf("%d", &receiverPhone);

    FILE *fp = fopen("accounts.txt", "r+");

    // Check if sender account exists and has sufficient balance
    while (fread(&sender, sizeof(struct Account), 1, fp))
    {
        if (sender.phone == senderPhone && strcmp(sender.password, senderPassword) == 0)
        {
            printf("Enter Amount to Transfer: $");
            scanf("%f", &amount);

            if (amount > sender.balance)
            {
                printf("\n**Insufficient Balance!**\n");
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
        if (receiver.phone == receiverPhone)
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
        printf("\n==Transfer Successful!==\n");
    }
    else
    {
        printf("\n **Transfer Failed! Check account details.**\n");
    }
}

// Function to view account status
void accountStatus()
{
    int phone;
    char password[20];
    struct Account acc;
    int found = 0;

    printf("Enter Phone Number: ");
    scanf("%d", &phone);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("accounts.txt", "r");
    while (fread(&acc, sizeof(struct Account), 1, fp))
    {
        if (acc.phone == phone && strcmp(acc.password, password) == 0)
        {
            printf("\n=== Account Status ===\n");
            printf("Name: %s\n", acc.name);
            printf("Account Number: %d\n", acc.phone);
            printf("Current Balance: $%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
    {
        printf("\n**Account not found or incorrect password.**\n");
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
