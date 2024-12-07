#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#define BOUQUETSTOCK 8
#define FLOWERSTOCK 10
void addToCart(char item_name[], int quantity, float price);
void viewCart();
void expandCart();


int i; //declaration of i for for loop
typedef struct {
    char name[20];
    char color[20];
    float price; 
} flower;

typedef struct {
    char name[20];
    char description[100];
    float price;
} bouquet;

typedef struct {
    char name[50];
    int quantity;
    float total_price;
} cart_item; 

cart_item* cart = NULL; // Pointer to dynamically allocated cart
int cart_index = 0; // Tracks the number of items in the cart
int cart_size = 0; // Tracks the current allocated size of the cart


void signup() {
    char stored_username[100];
    char stored_password[100];
    char confirm_password[100];
    unsigned long phone_number;

    printf("Enter username: \n");
    scanf("%s", stored_username);

    while (1) {
        printf("Enter password: \n");
        scanf("%s", stored_password);

        printf("Confirm password: \n");
        scanf("%s", confirm_password);

        if (strcmp(stored_password, confirm_password) == 0) {
            printf("Signup successful!\n");
            break;
        } else {
            printf("Passwords do not match. Please try again.\n");
        }
    }

    printf("Enter your phone number: \n");
    scanf("%lu", &phone_number);

    FILE *file = fopen("login", "a"); 
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
    } else {
        fprintf(file, "%s %s %lu\n", stored_username, stored_password, phone_number);//store user information
        fclose(file);
        printf("Signup successful! You can login now.\n");
    }
}

int login() {
    struct user {
        char entered_username[100];
        char entered_password[100];
        char stored_username[100];
        char stored_password[100];
        unsigned long phone_number;
    } user;

    int match = 0;

    FILE *file = fopen("login", "r");
    if (file == NULL) {
        printf("Error: Could not open file for reading.\n");//user can login through their credentials. this function will check it and allow the user.
        return 0; // Unable to login
    }

    printf("Enter username: ");
    scanf("%s", user.entered_username);
    printf("Enter password: ");
    scanf("%s", user.entered_password);

    while (fscanf(file, "%s %s %lu", user.stored_username, user.stored_password, &user.phone_number) != EOF) {
        if (strcmp(user.entered_username, user.stored_username) == 0 && strcmp(user.entered_password, user.stored_password) == 0) {
            match = 1;
            break;
        }//reading users credentials from file.
    }

    fclose(file);  

    if (match) {
        printf("Login successful! You can proceed further.\n");
        return 1; // Login successful
    } else {
        printf("Invalid credentials.\n");
        return 0; // Login failed
    }
}

void bouquetdisplay(bouquet premade[], int count) {
    printf("\n--- Pre-Made Bouquets ---\n");
    for (i = 0; i < count; i++) {
        printf("%d. %s - %s ($%.2f)\n", 
               i + 1, 
               premade[i].name, 
               premade[i].description, 
               premade[i].price);
    }
}

void flowersdisplay(flower flowers[], int count) {
    printf("\n--- Available Flowers ---\n");
    for (i = 0; i < count; i++) {
        printf("%d. %s (%s) - $%.2f\n", 
               i + 1, 
               flowers[i].name, 
               flowers[i].color, 
               flowers[i].price);
    }
}

void shopBouquets(bouquet premade[], int count) {
    int bouquet_choice, bouquet_quantity;
    bouquetdisplay(premade, count);
    while (1) {
        printf("Enter the bouquet number to add to cart (0 to finish): ");
        scanf("%d", &bouquet_choice);

        if (bouquet_choice == 0) {
            printf("Finished shopping for bouquets.\n");
            break;
        } else if (bouquet_choice >= 1 && bouquet_choice <= count) {
            printf("Enter quantity: ");
            scanf("%d", &bouquet_quantity);
            addToCart(
                premade[bouquet_choice - 1].name, 
                bouquet_quantity, 
                premade[bouquet_choice - 1].price
            );
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}


void shopFlowers(flower flowers[], int count) {
    int flower_choice, flower_quantity;
    flowersdisplay(flowers, count);

    while (1) {
        printf("Enter the flower number to add to cart (0 to finish): ");
        scanf("%d", &flower_choice);

        if (flower_choice == 0) {
            printf("Finished shopping for flowers.\n");
            break;
        } else if (flower_choice >= 1 && flower_choice <= count) {
            printf("Enter quantity: ");
            scanf("%d", &flower_quantity);
            addToCart(flowers[flower_choice - 1].name, flower_quantity, flowers[flower_choice - 1].price);
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}


void initializeCart(int initial_size) {
    cart_size = initial_size;
    cart = (cart_item*)malloc(cart_size * sizeof(cart_item));
    if (cart == NULL) {
        printf("Error allocating memory for cart.\n");
        exit(1);
    }
}

void expandCart() {
    cart_size *= 2;
    cart_item* new_cart = (cart_item*)realloc(cart, cart_size * sizeof(cart_item));
    if (new_cart == NULL) {
        printf("Error reallocating memory for cart.\n");
        free(cart);
        exit(1);
    }
    cart = new_cart;
}


void addToCart(char item_name[], int quantity, float price) {
	if (cart_index == cart_size){
		expandCart();
	}
    strcpy(cart[cart_index].name, item_name);
    cart[cart_index].quantity = quantity;
    
    printf("adding item: %s | quantity: %d | price per unit: %.2f\n",item_name,quantity,price);
    
    cart[cart_index].total_price = quantity * price;
     printf("Total Price for item: %.2f\n", cart[cart_index].total_price);
    cart_index++;
    printf("\n%s added to cart successfully!\n", item_name);
}

void viewCart() {
    if (cart_index == 0) {
        printf("\nYour cart is empty. Add some items first.\n");
        return;
    }

    float totalBill = 0;
    printf("\n---- Your Cart ----\n");
    printf("%-5s %-30s %-10s %-10s\n", "No.", "Item Name", "Quantity", "Total Price");
    for (i = 0; i < cart_index; i++) {
        printf("%-5d %-30s %-10d $%-10.2f\n", 
               i + 1, cart[i].name, cart[i].quantity, cart[i].total_price);
        totalBill += cart[i].total_price;
    }
    printf("--------------------------------------------\n");
    printf("Total Bill: $%.2f\n", totalBill);
}


void CheckOut(){ //displays order with total bill , ask for address, ask for payment method confirm payment method , confirm order and ask to exit...

    int paymentMethod;
    int cardType;
    char cardNumber[20];
    float discount = 0.0;	
    int choice;
    
    if (cart_index == 0) {
        printf("\nYour cart is empty. Please add some items first.\n");
        return;
    }
    
    float totalBill = 0;
    for ( i = 0; i < cart_index; i++) {
        totalBill += cart[i].total_price;
    }
    
    printf("\n---- Checkout ----\n");
    viewCart();
    
	char* address = (char*)malloc(150 * sizeof(char));//allocating memory for the address.
    if (address == NULL) {
        printf("Error allocating memory for address.\n");
        return;
    }
	getchar();     
	printf("Please enter your address: \n");
	 
    fgets(address, 200, stdin);
    address[strcspn(address, "\n")] = '\0';
    
    printf("Address saved: %s\n", address);

	FILE* orderFile = fopen("order_history.txt", "a");//created a file to show the order history to the user.
    if (orderFile == NULL) {
        printf("Error saving order history.\n");
    } else {
        fprintf(orderFile, "Order Details:\n");
        fprintf(orderFile, "Address: %s\n", address);
        fprintf(orderFile, "%-30s %-10s %-10s\n", "Item Name", "Quantity", "Total Price");
        for (i = 0; i < cart_index; i++) {
            fprintf(orderFile, "%-30s %-10d $%-10.2f\n", cart[i].name, cart[i].quantity, cart[i].total_price);
        }
        fprintf(orderFile, "Total Bill: $%.2f\n", totalBill);
        fprintf(orderFile, "--------------------------------------------\n");
        fclose(orderFile);
    }
	
	printf("Please confirm your payment method\n");
	printf("1) CASH ON DELIVERY\n");
	printf("2) CARD PAYMENT\n");
	scanf("%d",&paymentMethod);
	
	if (paymentMethod == 1){
		printf("Your order has been confirmed. It will deliver to %s in 2 weeks. \n",address);
		printf("Your total  bill is: %.2f \n", totalBill);
		printf("Thankyou for shopping from us :)\n");
	}
	else if(paymentMethod == 2) {
		printf("\n you chose card payment\n");
		printf("choose your card type\n");
		printf("1) VISA\n");
		printf("2) MASTERCARD\n");
		printf("3) PAYPAK\n");
		scanf("%d",&cardType);
    
		switch(cardType){  //applying discount on the total bill as per their card type.
			case 1:{
				printf("you chose visa card. 15%% discount will be applied to your total bill.\n");
				discount = 0.15 * totalBill;
				break;
			}
			case 2: {
				printf("You chose mastercard. 10%% discount will be applied to your total bill.\n");
				discount = 0.10 * totalBill;
				break;
			}
			case 3: {
				printf("You chose paypak. 5%% discount will be applied to your total bill.\n");
				discount = 0.05 * totalBill;
				break;
			}
			default: {
				printf("Invalid card type\n");
				return;
			}
		}
		printf("Please enter your card number (only last 4 digits): \n");
	    scanf(" %s",cardNumber);
	    printf("Card accepted. Processing payment...\n");
	
      	float FinalAmount = totalBill - discount;
	    printf("Address: %s\n",address);
      	printf("Your card number is: %s \n",cardNumber);
    	printf("Applied discount:%.2f \n",discount);
    	printf("Your total bill is: %.2f \n",FinalAmount);
    	printf("Your order has been placed. It will deliver to %s in 2 weeks.\n",address);
    	printf("Thankyou for shopping from us!.\n");
	} else {
		printf("Invalid payment method\n");
	}
	free(address);
}
	
void ViewOrderHistory() {
    FILE* orderFile = fopen("order_history.txt", "r");
    if (orderFile == NULL) {
        printf("No order history found.\n");
        return;
    }

    printf("\n---- Order History ----\n");
    char line[256];
    while (fgets(line, sizeof(line), orderFile) != NULL) {
        printf("%s", line);
    }
    fclose(orderFile);
}

int main() {
    bouquet premade[BOUQUETSTOCK] = {
        {"Eternal Love", "A bouquet of red roses with baby's breath.", 29.99},
        {"Sunny Delight", "A mix of sunflowers and daisies.", 24.99},
        {"Elegant White", "A classy arrangement of white lilies.", 34.99},
        {"Vivid Vibrance", "A colorful mix of tulips and carnations.", 27.99},
        {"Blissful Beauty", "A cheerful mix of assorted fresh flowers.", 22.99},
        {"Heart's Desire", "A mix of red tulips, pink carnations, and white lilies.", 32.50},
        {"Golden Hour", "Peach roses, apricot carnations, and golden chrysanthemums.", 38.70},
        {"Forest Serenity", "Green hydrangeas, white orchids, and ferns.", 25.99}
    };

    flower flowers[FLOWERSTOCK] = {
        {"Roses", "Red", 1.99}, {"Roses", "White", 1.99}, {"Lilies", "White", 2.50},
        {"Sunflowers", "Yellow", 2.99}, {"Tulips", "Pink", 1.50}, {"Carnations", "Apricot", 1.50},
        {"Hydrangeas", "Green", 2.50}, {"Orchids", "White", 3.50}, {"Chrysanthemums", "Golden", 2.25},
        {"Daisies", "White", 1.50}
    };

    initializeCart(10); // Initialize the cart with 10 items

    int choice, loggedIn = 0;

    while (1) {
        if (!loggedIn) {
            printf("\n\nWelcome to Blooms and Giggles\n\n");
            printf("1) SIGNUP\n");
            printf("2) LOGIN\n");
            printf("3) EXIT\n\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    signup();
                    break;
                case 2:
                    loggedIn = login();
                    break;
                case 3:
                    free(cart);
                    printf("Exiting the system. Thanks!\n");
                    exit(0);
                default:
                    printf("\nPlease enter a valid choice.\n");
            }
        } else {
            printf("\n\nWhat would you like to do?\n");
            printf("1) Shop Pre-made Bouquets\n");
            printf("2) Customize Your Own Bouquet (Flowers)\n");
            printf("3) View Cart\n");
            printf("4) Checkout\n");
            printf("5) View Order History\n");
            printf("6) Logout\n\n");

            int shop_choice;
            printf("Enter your choice: ");
            scanf("%d", &shop_choice);

            switch (shop_choice) {
                case 1:
                    shopBouquets(premade, BOUQUETSTOCK);
                    break;
                case 2:
                    shopFlowers(flowers, FLOWERSTOCK);
                    break;
                case 3:
                    viewCart();
                    break;
                case 4:
                    CheckOut();
                    free(cart); // Free cart memory after checkout
                    loggedIn = 0; // Log out after checkout
                    break;
                case 5:
	                ViewOrderHistory();
	                break;
                case 6:
                    printf("Logging out. Thank you for visiting Blooms and Giggles!\n");
                    loggedIn = 0;
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    }

    return 0;
}

                   

