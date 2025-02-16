#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include<ctype.h>


int is_valid_name(const char *name) /*function definition to check the enter name is valid or not , the name only contain alphabets */
{
    for (int i = 0; i < strlen(name); i++) 
    {
        if (!isalpha(name[i]) && name[i] != ' ') 
        { // Check if it's not alphabetic and not a space
            return 0; // Invalid name
        }
    }
    return 1; // Valid name
}

int is_valid_phone(const char *phone) /*function defintion to check the enter phone is valid ot not it should be a 10 digit number */
{
    // Check if the phone number is exactly 10 digits
    if (strlen(phone) != 10) 
    {
        return 0; // Invalid length
    }

    // Check if all characters are digits
    for (int i = 0; i < 10; i++) 
    {
        if (!isdigit(phone[i])) 
        {
            return 0; // Invalid character found, return 0
        }
    }

    return 1;
}

int is_duplicate_phone(const AddressBook *addressBook, const char *phone) /*Function to check for duplicate phone number if already present */
{
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) //Compare the eneter number with stored numbers in addressbook
        {
            return 1; // Duplicate found
        }
    }
    return 0; // No duplicate
}


int is_valid_email(const char *email) /*function definition to check enter email is valid or not */
{
    char *at_char = strchr(email,'@'); //check for character @
    char *dot_char = strstr(email,".com"); //check for string .com
    if(!at_char || !dot_char) //if @ and .com not found
    {
        return 0;
    }

    if(dot_char <= at_char + 1) //check for character between @ and .com
    {
        return 0;
    }

    if(at_char - email < 1) //check for any character before @
    {
        return 0;
    }
    return 1; 
}
int is_duplicate_email(const AddressBook *addressBook, const char *email) {
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcasecmp(addressBook->contacts[i].email, email) == 0) { // Case-insensitive comparison
            return 1; // Duplicate found
        }
    }
    return 0; // No duplicate
}


void listContacts(AddressBook *addressBook)  //Function to list the contacts
{
int i;
for(i = 0; i < addressBook -> contactCount ; i++)
{
printf("Name = \t%s\t\t",addressBook -> contacts[i].name);
printf("phone = %s\t\t",addressBook -> contacts[i].phone);
printf("email = %s\t\n",addressBook -> contacts[i].email);
}
    
}

void initialize(AddressBook *addressBook)  //function to intialize and load contact
{
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
    
    // Load contacts from file during initialization
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
/* Define the logic to create a Contacts */
    
    /* Check if the address book is full */
    if (addressBook->contactCount >= MAX_CONTACTS) //If the addressbook reached the maximum limit of contact to be stroed
    {
        printf("Address book is full! Cannot add more contacts.\n");
        return;
    }

    Contact newcontact;

    // Input and validate name
input_name:
    printf("Enter the name: ");
    scanf(" %[^\n]", newcontact.name); 
    if (!is_valid_name(newcontact.name)) //Function call to validate name 
    {
        printf("Invalid name, Name must contain only letters and spaces. Please try again.\n");
        goto input_name;  // Go back to name input if validation fails
    }

    // Input and validate phone number
input_phone:
    printf("Enter the phone number (10 digits): ");
    scanf("%s", newcontact.phone);
    if (!is_valid_phone(newcontact.phone)) //Function call to validate phone
    {
        printf("Invalid phone number. It should contain exactly 10 digits. Please try again.\n");
        goto input_phone;  // Go back to phone input if validation fails
    }
    if (is_duplicate_phone(addressBook, newcontact.phone)) //Check for duplicate phone number in contact addressbook
    {
        printf("This phone number is already in the address book. Please enter a different phone number.\n");
        goto input_phone;
    }

    // Input and validate email
input_email:
    printf("Enter the email: ");
    scanf("%s", newcontact.email);
    if (!is_valid_email(newcontact.email)) //Function call to validate email
    {
        printf("Invalid email format. The email should contain '@' and '.com'. Please try again.\n");
        goto input_email;  // Go back to email input if validation fails
    }
     if (is_duplicate_email(addressBook, newcontact.email)) 
    { // Check for duplicate email
        printf("This email is already in the address book. Please enter a different email.\n");
        goto input_email;
    }

    // Add the new contact to the address book
    addressBook->contacts[addressBook->contactCount] = newcontact;
    addressBook->contactCount++;

    printf("Contact added successfully!\n");

}



void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */ 
    int searchoption; //declare the variables
    char searchterm[50];
    search :
    printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter your choice : ");
    scanf("%d", &searchoption); //Promt the user to enter the option which he want to serch contact

    switch (searchoption) //case to search contact based on user preference
    {
        case 1: //To search contact by using name
input_name:
            printf("Enter the name: ");
            scanf(" %[^\n]", searchterm); 
            if (!is_valid_name(searchterm))  //Validate name 
            {
                printf("Invalid name. Name must contain only letters and spaces. Please try again.\n");
                goto input_name;  // Go back to name input if invalid
            }
            break;

        case 2:   //To search contact by using phone
input_phone:
            printf("Enter the phone number: ");
            scanf(" %[^\n]", searchterm); 
            if (!is_valid_phone(searchterm)) //validate phone
            {
                printf("Invalid phone number. Try again.\n");
                goto input_phone;  // Go back to phone input if invalid
            }
            break;
        
        case 3: // To search contact by using email
input_email:
            printf("Enter the email: ");
            scanf(" %[^\n]", searchterm); 
            if (!is_valid_email(searchterm)) //validate email
            {
                printf("Invalid email. Try again.\n");
                goto input_email;  // Go back to email input if invalid
            }
            break;

        default:
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            goto search;
    }

    // Search through the contacts
    int found[100];
    int foundcount = 0; //Intialize the found
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        /* Check for stored contact in the file by compare name, phone and email */
        if ((searchoption == 1 && strcasecmp(addressBook->contacts[i].name, searchterm) == 0) ||
            (searchoption == 2 && strcmp(addressBook->contacts[i].phone, searchterm) == 0) ||
            (searchoption == 3 && strcasecmp(addressBook->contacts[i].email, searchterm) == 0)) 
        {    
            found[foundcount++] = i;
        }
    }

    if(foundcount == 0) //If entered details not match with stored contact
    {
        printf("contact not found\n");
        //goto input_name;
        return;
    }
    if(foundcount == 1) //if single contact found based on provided details
    {
            int i = found[0];
            printf("Contact found:\n");
            printf("Name: %s\n", addressBook->contacts[i].name);
            printf("Phone: %s\n", addressBook->contacts[i].phone);
            printf("Email: %s\n", addressBook->contacts[i].email);     
    }

    /*If more than one contact found based on provided detailes then search the contact by phone number */
    else if (searchoption == 1 && foundcount > 1)  
    {
        printf("Multiple contacts found with the name \"%s\". Please enter the phone number:\n", searchterm);
        char searchPhone[15];
input_phone_again:
        printf("Enter the phone number: "); //Promt the user to enetr the phone number
        scanf(" %[^\n]", searchPhone);
        if (!is_valid_phone(searchPhone)) //Validate the number 
        {
            printf("Invalid phone number. Try again.\n");
            goto input_phone_again;
        }

        // Search within the found results using the phone number
        for (int i = 0; i < foundcount; i++) 
        {
            int index = found[i];
            if (strcmp(addressBook->contacts[index].phone, searchPhone) == 0) //display the contact found
            {
                printf("Contact found:\n");
                printf("Name: %s\n", addressBook->contacts[index].name);
                printf("Phone: %s\n", addressBook->contacts[index].phone);
                printf("Email: %s\n", addressBook->contacts[index].email);
                return;
            }
        }

        printf("No contact found with the provided phone number.\n");
    }

}

void editContact(AddressBook *addressBook)
{
    char searchname[50];  
    char searchphone[15];  // Stores the phone number if needed for multiple matches
    int found = -1; 
    int matchcount = 0;  // Tracks the number of matches found

input_name:
    printf("Enter the name: ");
    scanf(" %[^\n]", searchname);  // Read the name input from the user
    if (!is_valid_name(searchname)) {  // Validate the name
        printf("Invalid name. Name must contain only letters and spaces. Please try again.\n");
        goto input_name;  // Ask for the name again if validation fails
    }

    matchcount = 0;
    // Search for contacts matching the input name
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].name, searchname) == 0) {
            matchcount++;  // Count the matches
            if (matchcount == 1) 
            {  
                found = i;
            }
        }
    }

    //case when multiple contacts with the same name are found
    if (matchcount > 1) {
input_phone:
        printf("Multiple contacts found with the name \"%s\". Please enter the phone number to identify the contact:\n", searchname);
        printf("Enter the phone number (10 digits): ");
        scanf("%s", searchphone);  // Read the phone number input
        if (!is_valid_phone(searchphone)) {  // Validate the phone number
            printf("Invalid phone number. It should contain exactly 10 digits. Please try again.\n");
            goto input_phone;  // Ask for the phone number again if validation fails
        }

        found = -1;
        // Search for a contact with the same name and phone number
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcmp(addressBook->contacts[i].name, searchname) == 0 && strcmp(addressBook->contacts[i].phone, searchphone) == 0) {
                found = i; 
                break;
            }
        }

        if (found == -1) 
        {  // If no contact found with the phone number
            printf("No contact found with the provided phone number.\n");
            goto input_name;  // Ask for the name again
        }
    } 
    else if (matchcount == 0) 
    {  // No contact found with the input name
        printf("No contact found with the name \"%s\".\n", searchname);
        goto input_name;  // Ask for the name again
    }

    // Display the contact details once found
    printf("Contact found:\nName: %s\nPhone: %s\nEmail: %s\n", addressBook->contacts[found].name, addressBook->contacts[found].phone, addressBook->contacts[found].email);

edit_choice:
    printf("What would you like to edit?\n1. Name\n2. Phone\n3. Email\nEnter your choice: ");
    int editChoice;
    scanf("%d", &editChoice);  // Get the user's choice for editing

    switch (editChoice) {
        case 1:  // Edit the name
edit_name:
            printf("Enter the new name: ");
            scanf(" %[^\n]", addressBook->contacts[found].name);  // Read the new name
            if (!is_valid_name(addressBook->contacts[found].name)) {  // Validate the new name
                printf("Invalid name format. Name must contain only letters and spaces. Please try again.\n");
                goto edit_name;  // Ask for the name again if validation fails
            }
            break;
        case 2:  // Edit the phone number
edit_phone:
            printf("Enter the new phone number (10 digits): ");
            scanf("%s", addressBook->contacts[found].phone);  // Read the new phone number
            if (!is_valid_phone(addressBook->contacts[found].phone)) {  // Validate the new phone number
                printf("Invalid phone number format. It should contain exactly 10 digits. Please try again.\n");
                goto edit_phone;  // Ask for the phone number again if validation fails
            }
            // Check for duplicate phone numbers in the address book
            for (int i = 0; i < addressBook->contactCount; i++) {
                if (i != found && strcmp(addressBook->contacts[i].phone, addressBook->contacts[found].phone) == 0) {
                    printf("This phone is already in the address book. Please enter a different phone number.\n");
                    goto edit_phone;  // Ask for the phone number again if duplicate is found
                }
            }
            break;
        case 3:  // Edit the email
edit_email:
            printf("Enter the new email: ");
            scanf("%s", addressBook->contacts[found].email);  // Read the new email
            if (!is_valid_email(addressBook->contacts[found].email)) {  // Validate the new email
                printf("Invalid email format. Email should contain '@' and '.com'. Please try again.\n");
                goto edit_email;  // Ask for the email again if validation fails
            }
            // Check for duplicate emails in the address book
            for (int i = 0; i < addressBook->contactCount; i++) {
                if (i != found && strcmp(addressBook->contacts[i].email, addressBook->contacts[found].email) == 0) {
                    printf("This email is already in the address book. Please enter a different email.\n");
                    goto edit_email;  // Ask for the email again if duplicate is found
                }
            }
            break;
        default:  // Invalid option for editing
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            goto edit_choice;  // Ask for the choice again if invalid
    }

    printf("Contact updated successfully!\n"); 
}



void deleteContact(AddressBook *addressBook)
{
/* Define the logic for deletecontact */
    int searchoption;
    char searchterm[50];
    
    printf("Delete by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter your choice (1/2/3): ");
    scanf("%d", &searchoption);

    switch (searchoption) 
    {
        case 1:
input_name:
            printf("Enter the name: "); 
            scanf(" %[^\n]", searchterm); //Read name from user
            /* validate name */
            if (!is_valid_name(searchterm)) 
            {
                printf("Invalid name. Please try again.\n");
                goto input_name;  // Go back to name input if invalid
            }
            break;

        case 2:
input_phone:
            printf("Enter the phone number: ");
            scanf(" %[^\n]", searchterm); //Read phne from user
            /* Validate phone */
            if (!is_valid_phone(searchterm)) 
            {
                printf("Invalid phone number. Please try again.\n");
                goto input_phone;  // Go back to phone input if invalid
            }
            break;
        
        case 3:
input_email:
            printf("Enter the email: ");
            scanf(" %[^\n]", searchterm); //Read email from user
            /* Validate email */
            if (!is_valid_email(searchterm)) {
                printf("Invalid email. Please try again.\n");
                goto input_email;  // Go back to email input if invalid
            }
            break;

        default:
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            return;
    }

    int foundindices[10];  // Array to store indices of matching contacts
    int foundcount = 0;    // Number of matching contacts

    // Search for the contact to delete
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        /* caompare the entered option and deatils with existing contact in addressbook */
        if ((searchoption == 1 && strcmp(addressBook->contacts[i].name, searchterm) == 0) ||
            (searchoption == 2 && strcmp(addressBook->contacts[i].phone, searchterm) == 0) ||
            (searchoption == 3 && strcmp(addressBook->contacts[i].email, searchterm) == 0)) 
        {
            foundindices[foundcount++] = i;
        }
    }

    if (foundcount == 0)  //No contact found with provided details
    {
        printf("Contact not found.\n");
        return;
    }
    else if (foundcount > 1 && searchoption == 1) 
    {
        // Multiple contacts with the same name found, ask for phone number
        printf("Multiple contacts with the same name found. \nPlease enter the phone number: ");
        scanf(" %[^\n]", searchterm);

        int phoneMatchIndex = -1;
        for (int i = 0; i < foundcount; i++) 
        {
            if (strcmp(addressBook->contacts[foundindices[i]].phone, searchterm) == 0) 
            {
                phoneMatchIndex = foundindices[i];
                break;
            }
        }

        if (phoneMatchIndex == -1) 
        {
            printf("Contact with that phone number not found.\n");
            return;
        }

        // Delete the contact
        for (int i = phoneMatchIndex; i < addressBook->contactCount - 1; i++) 
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];  // Shift contact to the left
        }
        
        addressBook->contactCount--;  // Reduce contact count by 1
        printf("Contact deleted successfully!\n");
    } 
    else 
    {
        // Single contact found or search by phone, delete directly
        int foundIndex = foundindices[0];
        for (int i = foundIndex; i < addressBook->contactCount - 1; i++) 
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];  // Shift contact to the left
        }

        addressBook->contactCount--;  // Reduce contact count by 1
        printf("Contact deleted successfully!\n");
    }
 
}

