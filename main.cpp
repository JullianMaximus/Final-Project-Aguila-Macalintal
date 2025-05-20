#include <iostream>
#include <vector>
#include <string>

using namespace std;

class PricingStrategy {
public:
    virtual double calculateCost(double price, int quantity) const = 0;
    virtual ~PricingStrategy() {}
};

class NormalPricing : public PricingStrategy {
public:
    double calculateCost(double price, int quantity) const override {
        return price * quantity;
    }
};

class DiscountPricing : public PricingStrategy {
public:
    double calculateCost(double price, int quantity) const override {
        return price * quantity * 0.8;
    }
};

class Product {
protected:
    string name;
    double price;

public:
    Product(const string& name, double price) : name(name), price(price) {
        if (price < 0) {
            cout << "Price cannot be negative.\n";
            this->price = 0;
        }
    }

    virtual double calculateCost(int quantity, const PricingStrategy& strategy) const {
        return strategy.calculateCost(price, quantity);
    }

    virtual void display() const {
        cout << name << " - $" << price;
    }

    virtual void update(const string& newName, double newPrice) {
        if (newPrice < 0) {
            cout << "Price cannot be negative.\n";
            return;
        }
        name = newName;
        price = newPrice;
    }

    virtual string getName() const { return name; }
    virtual double getPrice() const { return price; }

    virtual ~Product() {}
};

class Clothing : public Product {
private:
    int stock;

public:
    Clothing(const string& name, double price, int stock)
        : Product(name, price), stock(stock) {}

    int getStock() const { return stock; }

    void reduceStock(int qty) {
        if (qty <= stock) stock -= qty;
    }

    void increaseStock(int qty) { stock += qty; }

    void display() const override {
        cout << name << " - $" << price << " (Available: " << stock << ")";
    }
};

class CartItem {
public:
    Product* product;
    int quantity;

    CartItem(Product* product, int quantity) : product(product), quantity(quantity) {}

    ~CartItem() {
        delete product;
    }

    double subtotal(const PricingStrategy& normal, const PricingStrategy& discount) const {
        const PricingStrategy& strategy = (quantity >= 3) ? discount : normal;
        return product->calculateCost(quantity, strategy);
    }

    void display(const PricingStrategy& normal, const PricingStrategy& discount) const {
        double cost = subtotal(normal, discount);
        cout << product->getName() << " x" << quantity << " - $";
        cout.precision(2);
        cout << fixed << cost;
        if (quantity >= 3) {
            cout << " (20% discount applied for 3 or more items)";
        }
        cout << endl;
    }
};

class ShoppingCart {
private:
    vector<CartItem*> items;
    NormalPricing normal;
    DiscountPricing discount;

public:
    void addProduct(Product* product, int quantity) {
        if (quantity <= 0) {
            cout << "Quantity must be at least 1.\n";
            delete product;
            return;
        }
        for (auto& item : items) {
            if (item->product->getName() == product->getName()) {
                item->quantity += quantity;
                cout << "Updated quantity of " << product->getName() << " to " << item->quantity << ".\n";
                delete product;
                return;
            }
        }
        items.push_back(new CartItem(product, quantity));
        cout << product->getName() << " added to cart with quantity " << quantity << ".\n";
    }

    void displayCart() const {
        if (items.empty()) {
            cout << "Cart is empty.\n";
            return;
        }
        cout << "\n--- Shopping Cart ---\n";
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". ";
            items[i]->display(normal, discount);
        }
    }

    void removeProduct(int index) {
        if (index >= 0 && index < (int)items.size()) {
            delete items[index];
            items.erase(items.begin() + index);
            cout << "Product removed.\n";
        } else {
            cout << "Invalid product index.\n";
        }
    }

    double calculateTotal() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item->subtotal(normal, discount);
        }
        return total;
    }

    void generateReceipt() const {
        if (items.empty()) {
            cout << "Cart is empty. Nothing to checkout.\n";
            return;
        }
        cout << "\n--- Receipt ---\n";
        for (const auto& item : items) {
            item->display(normal, discount);
        }
        cout.precision(2);
        cout << fixed << "Total: $" << calculateTotal() << "\n";
        cout << "Thank you for shopping with us!\n";
    }

    const vector<CartItem*>& getItems() const {
        return items;
    }

    void clearCart() {
        for (auto& item : items) {
            delete item;
        }
        items.clear();
        cout << "Cart has been cleared.\n";
    }

    ~ShoppingCart() {
        clearCart();
    }
};

class User {
public:
    string username;
    string password;

    User(const string& uname, const string& pwd) : username(uname), password(pwd) {}
};

class UserSystem {
private:
    vector<User> users;

public:
    void signup(const string& uname, const string& pwd) {
        for (const auto& u : users) {
            if (u.username == uname) {
                cout << "Username already exists.\n";
                return;
            }
        }
        if (uname.empty() || pwd.empty()) {
            cout << "Username and password cannot be empty.\n";
            return;
        }
        users.push_back(User(uname, pwd));
        cout << "Signup successful! Please login to continue.\n";
    }

    bool login(const string& uname, const string& pwd) const {
        for (const auto& u : users) {
            if (u.username == uname && u.password == pwd) {
                cout << "Login successful!\n";
                return true;
            }
        }
        cout << "Invalid username or password.\n";
        return false;
    }

    bool noUsers() const {
        return users.empty();
    }
};

void safeInputInt(int& var, const string& prompt) {
    while (true) {
        cout << prompt;
        cin >> var;
        if (cin.fail()) {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Invalid input. Please enter a number.\n";
        } else {
            while (cin.get() != '\n');
            break;
        }
    }
}

int main() {
    UserSystem userSystem;
    ShoppingCart cart;
    
    string choiceStr;
    int choice = 0;
    string uname, pwd;

    while (true) {
    cout << "1. Signup\n2. Login\nChoose: ";
    getline(cin, choiceStr);

    if (choiceStr != "1" && choiceStr != "2") {
        cout << "Invalid choice. Please enter 1 or 2.\n";
        continue;
    }

    choice = stoi(choiceStr);

    cout << "Username: ";
    cin >> uname;
    cout << "Password: ";
    cin >> pwd;
    cin.ignore(); 

    if (choice == 1) {
        userSystem.signup(uname, pwd);
        cout << "Please login now.\n";
    } else if (choice == 2) {
        if (userSystem.noUsers()) {
            cout << "No accounts found. Please create an account first.\n";
            continue; 
        }
        if (userSystem.login(uname, pwd)) {
            break; 
        } else {
            cout << "Login failed. Try again.\n";
        }
    }
}

    vector<Product*> storeItems = {
        new Clothing("Vintage Tee", 14.99, 10),
        new Clothing("Graphic Shirt", 19.99, 5),
        new Clothing("Plain White Tee", 9.99, 20),
        new Clothing("Denim Shirt", 24.50, 7),
        new Clothing("Polo Shirt", 18.75, 15)
    };

    int opt;
    bool shopping = true;

    while (shopping) {
        cout << "\n--- Online Store Menu ---\n";
        cout << "Available Shirts:\n";
        for (size_t i = 0; i < storeItems.size(); ++i) {
            cout << i + 1 << ". ";
            storeItems[i]->display();
            cout << "\n";
        }

        cout << "\nCurrent Cart Items:\n";
        cart.displayCart();

        safeInputInt(opt, "\n1. Add to Cart\n2. View Cart\n3. Remove from Cart\n4. Checkout\n5. Exit\nChoice: ");

        if (opt == 1) {
            char more = 'y';
            while (more == 'y' || more == 'Y') {
                int idx, qty;

                safeInputInt(idx, "\nEnter product index to add: ");
                if (idx >= 1 && idx <= (int)storeItems.size()) {
                    Clothing* shirt = dynamic_cast<Clothing*>(storeItems[idx - 1]);
                    if (!shirt) {
                        cout << "Invalid product.\n";
                        continue;
                    }

                    safeInputInt(qty, "Enter quantity: ");

                    if (qty <= 0) {
                        cout << "Quantity must be at least 1.\n";
                        continue;
                    }

                    if (qty > shirt->getStock()) {
                        cout << "Not enough stock! Available: " << shirt->getStock() << "\n";
                        continue;
                    }

                    cart.addProduct(new Clothing(shirt->getName(), shirt->getPrice(), 0), qty);
                    shirt->reduceStock(qty);
                } else {
                    cout << "Invalid product index.\n";
                }
                cout << "Do you want to add another item? (y/n): ";
                cin >> more;
            }
        } else if (opt == 2) {
            cart.displayCart();
        } else if (opt == 3) {
            cart.displayCart();
            int idx;
            safeInputInt(idx, "Enter index to remove: ");

            const auto& cartItems = cart.getItems();
            if (idx >= 1 && idx <= (int)cartItems.size()) {
                CartItem* item = cartItems[idx - 1];
                Clothing* shirtInStore = nullptr;
                for (auto& prod : storeItems) {
                    Clothing* c = dynamic_cast<Clothing*>(prod);
                    if (c && c->getName() == item->product->getName()) {
                        shirtInStore = c;
                        break;
                    }
                }
                if (shirtInStore) {
                    shirtInStore->increaseStock(item->quantity);
                }
                cart.removeProduct(idx - 1);
            } else {
                cout << "Invalid product index.\n";
            }
        } else if (opt == 4) {
            if (cart.getItems().empty()) {
                cout << "Your cart is empty. Nothing to checkout.\n";
            } else {
                cart.generateReceipt();
                cart.clearCart();

                cout << "\nDo you want to continue shopping? (y/n): ";
                char cont;
                cin >> cont;
                if (cont == 'n' || cont == 'N') {
                    shopping = false;
                    cout << "Thank you for shopping with us! Goodbye!\n";
                }
            }
        } else if (opt == 5) {
            shopping = false;
            cout << "Goodbye!\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    for (auto& item : storeItems) delete item;
    return 0;
}