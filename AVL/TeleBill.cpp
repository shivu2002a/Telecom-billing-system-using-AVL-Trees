#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Used during visualization
#define COUNT 10

using namespace std;

// Customer structure
struct Customer {
    string name;
    string address;
    string phoneNumber;
    double callDuration;
    double dataUsage;
    double totalBill;
};

// AVL Tree Node
struct Node {
    Customer customer;
    int height;
    Node* left;
    Node* right;
};

const int BLUE = 1;
const int GREEN = 2;
const int RED = 4;
const int WHITE = 15;


// Function to save customer data to a file
void saveDataToFile(Node* root, const string& filename);

// Helper function for saving data to a file (preorder traversal)
void saveDataToFileHelper(Node* node, ofstream& file);

// Function to get the height of a node
int getHeight(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Function to calculate the balance factor of a node
int getBalanceFactor(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// Function to update the height of a node
void updateHeight(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

// Function to perform a right rotation on a node
Node* rightRotate(Node* node) {
    Node* newRoot = node->left;
    Node* newRootRight = newRoot->right;

    // Perform rotation
    newRoot->right = node;
    node->left = newRootRight;

    // Update heights
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// Function to perform a left rotation on a node
Node* leftRotate(Node* node) {
    Node* newRoot = node->right;
    Node* newRootLeft = newRoot->left;

    // Perform rotation
    newRoot->left = node;
    node->right = newRootLeft;

    // Update heights
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// Function to balance the AVL tree
Node* balanceTree(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    updateHeight(node);

    int balanceFactor = getBalanceFactor(node);

    // Left-Left Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
        return rightRotate(node);
    }

    // Left-Right Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-Right Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
        return leftRotate(node);
    }

    // Right-Left Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Function to insert a customer into the AVL tree
Node* insertCustomer(Node* root, Customer customer) {
    if (root == nullptr) {
        Node* newNode = new Node;
        newNode->customer = customer;
        newNode->height = 1;
        newNode->left = nullptr;
        newNode->right = nullptr;
        cout << "Customer successfully inserted." << endl;
        return newNode;
    }

    if (customer.phoneNumber < root->customer.phoneNumber) {
        root->left = insertCustomer(root->left, customer);
    }
    else if (customer.phoneNumber > root->customer.phoneNumber) {
        root->right = insertCustomer(root->right, customer);
    } else {
        cout << "Error: Duplicate phone number. Customer not inserted." << endl;
        return root;
    }

    root = balanceTree(root);
    return root;
}

// Function to find the customer with the given phone number
Node* findCustomer(Node* root, string phoneNumber) {
    if (root == nullptr || root->customer.phoneNumber == phoneNumber) {
        return root;
    }

    if (phoneNumber < root->customer.phoneNumber) {
        return findCustomer(root->left, phoneNumber);
    }
    else {
        return findCustomer(root->right, phoneNumber);
    }
}

// Function to modify customer information
void modifyCustomer(Node* root, string phoneNumber) {
    Node* customerNode = findCustomer(root, phoneNumber);
    if (customerNode == nullptr) {
        cout << "Customer not found." << endl;
        return;
    }

    Customer& customer = customerNode->customer;

    cout << "Enter new name: ";
    getline(cin, customer.name);

    cout << "Enter new address: ";
    getline(cin, customer.address);

    cout << "Enter new call duration (minutes): ";
    cin >> customer.callDuration;

    cout << "Enter new data usage (MB): ";
    cin >> customer.dataUsage;

    // Recalculate total bill based on new information
    customer.totalBill = /* Calculation based on call duration, data usage, etc. */ (customer.dataUsage * 2) + (customer.callDuration * 60);

    cout << "Customer information updated." << endl;
}

// Function to delete a customer record
Node* deleteCustomer(Node* root, string phoneNumber, bool* flag) {
    if (root == nullptr) {
        return nullptr;
    }

    if (phoneNumber < root->customer.phoneNumber) {
        root->left = deleteCustomer(root->left, phoneNumber, flag);
    }
    else if (phoneNumber > root->customer.phoneNumber) {
        root->right = deleteCustomer(root->right, phoneNumber, flag);
    }
    else {
        // Found the customer to delete
        *flag = true;
        if (root->left == nullptr && root->right == nullptr) {
            // Case 1: Node to be deleted is a leaf node
            delete root;
            return nullptr;
        }
        else if (root->left == nullptr) {
            // Case 2: Node to be deleted has only a right child
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            // Case 3: Node to be deleted has only a left child
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else {
            // Case 4: Node to be deleted has both left and right children
            // Find the inorder successor (minimum value in the right subtree)
            Node* successor = root->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }

            // Copy the data of the successor to the current node
            root->customer = successor->customer;

            // Delete the successor node
            root->right = deleteCustomer(root->right, successor->customer.phoneNumber, flag);
        }
    }

    root = balanceTree(root);
    return root;
}

// Function to generate the bill for a customer
void generateBill(Node* root, string phoneNumber) {
    Node* customerNode = findCustomer(root, phoneNumber);
    if (customerNode == nullptr) {
        cout << "Customer not found." << endl;
        return;
    }

    Customer customer = customerNode->customer;
    // Calculate the bill based on call duration, data usage, etc.
    // Update the customer's totalBill field

    cout << "Customer Name: " << customer.name << endl;
    cout << "Phone Number: " << customer.phoneNumber << endl<< "Total Bill: " << customer.totalBill << endl;
}

// Function to process bill payment for a customer
Node* billPayment(Node* root, string phoneNumber) {
    Node* customerNode = findCustomer(root, phoneNumber);
    if (customerNode == nullptr) {
        cout << "Customer not found." << endl;
        return root;
    }

    Customer& customer = customerNode->customer;
    if(customer.totalBill == 0) {
        cout << "Bill is totally Paid !!";
        return root;
    }
    double amountPaid;
    cout << "Enter the amount to be paid: ";
    cin >> amountPaid;
    while(amountPaid > customer.totalBill) {
        cout << "Enter a valid amount: ";
        cin >> amountPaid;
    }
    customer.totalBill -= amountPaid;
    if (customer.totalBill < 0) {
        customer.totalBill = 0;
    }

    cout << "Payment of " << amountPaid << " successfully processed for customer: " << customer.name << endl;
    cout << "Remaining balance: " << customer.totalBill << endl;
    return root;
}


// Function to display the customers in ascending order of phone numbers (inorder traversal)
void displayCustomers(Node* root) {
    if (root == nullptr) {
        return;
    }

    displayCustomers(root->left);
    Customer customer = root->customer;
    cout << "Customer Name: " << customer.name << endl;
    cout << "Phone Number: " << customer.phoneNumber << endl;
    cout << "Address: " << customer.address << endl;
    cout << "----------------------------" << endl;
    displayCustomers(root->right);
}

// Function to save customer data to a file
void saveDataToFile(Node* root, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: Failed to open file for writing." << endl;
        return;
    }

    // Preorder traversal to save data
    saveDataToFileHelper(root, file);

    file.close();
    cout << "Data saved to file." << endl;
}

// Helper function for saving data to a file (preorder traversal)
void saveDataToFileHelper(Node* node, ofstream& file) {
    if (node == nullptr) {
        return;
    }

    Customer customer = node->customer;

    file << "Name: " << customer.name << endl;
    file << "Phone Number: " << customer.phoneNumber << endl;
    file << "Address: " << customer.address << endl;
    file << "Call Duration: " << customer.callDuration << endl;
    file << "Data Usage: " << customer.dataUsage << endl;
    file << "Total Bill: " << customer.totalBill << endl;
    file << "----------------------------" << endl;

    saveDataToFileHelper(node->left, file);
    saveDataToFileHelper(node->right, file);
}

// Function to load customer data from a file
Node* loadDataFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Failed to open file for reading." << endl;
        return nullptr;
    }

    Node* root = nullptr;
    string line;
    Customer customer;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (line.substr(0, 6) == "Name: ") {
            customer.name = line.substr(6);
        }
        else if (line.substr(0, 14) == "Phone Number: ") {
            customer.phoneNumber = line.substr(14);
        }
        else if (line.substr(0, 9) == "Address: ") {
            customer.address = line.substr(9);
        }
        else if (line.substr(0, 15) == "Call Duration: ") {
            customer.callDuration = stod(line.substr(15));
        }
        else if (line.substr(0, 12) == "Data Usage: ") {
            customer.dataUsage = stod(line.substr(12));
        }
        else if (line.substr(0, 12) == "Total Bill: ") {
            customer.totalBill = stod(line.substr(12));

            // Insert customer into AVL tree
            root = insertCustomer(root, customer);
        }
    }

    file.close();
    cout << "Data loaded from file." << endl;

    return root;
}

// Function to perform inorder traversal of the AVL tree
void inorderTraversal(Node* root) {
    if (root == nullptr) {
        return;
    }

    inorderTraversal(root->left);
    cout << "Phone Number: " << root->customer.phoneNumber << " | Total Bill: " << root->customer.totalBill << endl;
    inorderTraversal(root->right);
}
 
// Function to print binary tree in 2D
// It does reverse inorder traversal
void visualizeTree(Node* root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    visualizeTree(root->right, space);
 
    // Print current node after space
    // count
    cout << endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout << root->customer.phoneNumber << "\n";
 
    // Process left child
    visualizeTree(root->left, space);
}

// Function to display the AVL tree visualization
void displayTreeVisualization(Node* root) {
    cout << "================= AVL Tree Visualization =================" << endl;
    if(root == NULL) {
        cout << "Root is empty !!";
        return;
    }
    visualizeTree(root, 0);
    cout << "=========================================================" << endl;
}


// Function to display the menu options
void displayMenu() {
    cout << "=============== Telecom Billing System ===============" << endl;
    cout << "1. Add a customer" << endl;
    cout << "2. Search for a customer" << endl;
    cout << "3. Modify customer information" << endl;
    cout << "4. Delete a customer record" << endl;
    cout << "5. Display all customers" << endl;
    cout << "6. Generate bill" << endl;
    cout << "7. Bill Payment" << endl;
    cout << "8. Load data from file" << endl;
    cout << "9. Save data to a file" << endl;
    cout << "10. Inorder Traversal" << endl;
    cout << "11. AVL Tree visualization" << endl;
    cout << "0. Exit" << endl;
    cout << "====================================================" << endl;
    cout << "Enter your choice: ";
}

bool checkPhoneNumber(string s) {
    bool flag = false;
   for( int i = 0; i < s.length(); i++ ) {
      if( !isdigit( s[i] )) {
         return false;
      }
   }
   return true;
}

int main() {
    Node* root = nullptr;
    int choice;
    string phoneNumber;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore();  // Ignore the newline character

        switch (choice) {
            case 1: {
                Customer customer;
                cout << "Enter name: ";
                getline(cin, customer.name);

                cout << "Enter address: ";
                getline(cin, customer.address);

                do {
                    cout << "Enter phone number: ";
                    getline(cin, customer.phoneNumber);
                    if(customer.phoneNumber.length() != 10 || !checkPhoneNumber(customer.phoneNumber))
                        cout << "Invalid phone number !!\n";
                    else
                        break;
                } while(1); 

                cout << "Enter call duration (minutes): ";
                cin >> customer.callDuration;

                cout << "Enter data usage  (MB): ";
                cin >> customer.dataUsage;

                customer.totalBill = /* Calculation based on call duration, data usage, etc. */ (customer.dataUsage * 2) + (customer.callDuration * 60);

                root = insertCustomer(root, customer);
                break;
            }
            case 2: {
                cout << "Enter phone number to search: ";
                getline(cin, phoneNumber);

                Node* customerNode = findCustomer(root, phoneNumber);
                if (customerNode == nullptr) {
                    cout << "Customer not found." << endl;
                }
                else {
                    Customer customer = customerNode->customer;
                    cout << "Customer Name: " << customer.name << endl;
                    cout << "Phone Number: " << customer.phoneNumber << endl;
                    cout << "Address: " << customer.address << endl;
                }

                break;
            }
            case 3: {
                cout << "Enter phone number to modify: ";
                getline(cin, phoneNumber);

                modifyCustomer(root, phoneNumber);

                break;
            }
            case 4: {
                cout << "Enter phone number to delete: ";
                getline(cin, phoneNumber);
                bool flag = false;
                root = deleteCustomer(root, phoneNumber, &flag);
                if(flag)
                    cout << "Customer record deleted." << endl;
                else
                    cout << "Customer record not deleted. Record doesn't exist" << endl;
                break;
            }
            case 5: {
                displayCustomers(root);
                break;
            }
            case 6: {
                cout << "Enter phone number to generate bill: ";
                getline(cin, phoneNumber);

                generateBill(root, phoneNumber);

                break;
            }
            case 7: {
                cout << "Enter phone number for bill payment: ";
                getline(cin, phoneNumber);
                root = billPayment(root, phoneNumber);
                break;
            }
            case 8: {
                string filename;
                cout << "Enter file name to load data: ";
                getline(cin, filename);

                root = loadDataFromFile(filename);

                break;
            }
            case 9: {
                string filename;
                cout << "Enter file name to save data: ";
                getline(cin, filename);

                saveDataToFile(root, filename);

                break;
            }
            case 10: {
                inorderTraversal(root);
                break;
            }
            case 11: {
                displayTreeVisualization(root);
                break;
            }
            case 0: {
                cout << "Exiting program..." << endl;
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }

        cout << endl;
    } while (choice != 0);

    return 0;
}
