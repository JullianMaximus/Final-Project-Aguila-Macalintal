#include <iostream>
#include <string>
#include <vector>

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
    Product(const string& name, double price) : name(name), price(price) {}

    virtual double calculateCost(int quantity, const PricingStrategy& strategy) const {
        return strategy.calculateCost(price, quantity);
    }

    virtual void display() const {
        cout << name << " - $" << price;
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

    void increaseStock(int qty) {
        stock += qty;
    }

    void display() const override {
        cout << name << " - $" << price << " (Available: " << stock << ")";
    }
};

class CartItem {
    public:
        Product* product;
        int quantity;
    
        CartItem(Product* product, int quantity) : product(product), quantity(quantity) {}
        ~CartItem() { delete product; }
    
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
                cout << " (20% discount applied)";
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
            if (quantity <= 0) return;
            for (auto& item : items) {
                if (item->product->getName() == product->getName()) {
                    item->quantity += quantity;
                    delete product;
                    return;
                }
            }
            items.push_back(new CartItem(product, quantity));
        }
    
        void displayCart() const {
            for (size_t i = 0; i < items.size(); ++i) {
                cout << i + 1 << ". ";
                items[i]->display(normal, discount);
            }
        }
    
        void removeProduct(int index) {
            if (index >= 0 && index < (int)items.size()) {
                delete items[index];
                items.erase(items.begin() + index);
            }
        }
    
        double calculateTotal() const {
            double total = 0.0;
            for (const auto& item : items)
                total += item->subtotal(normal, discount);
            return total;
        }
    
        void generateReceipt() const {
            cout << "\n--- Receipt ---\n";
            for (const auto& item : items)
                item->display(normal, discount);
            cout.precision(2);
            cout << fixed << "Total: $" << calculateTotal() << "\n";
        }
    
        const vector<CartItem*>& getItems() const {
            return items;
        }
    
        void clearCart() {
            for (auto& item : items)
                delete item;
            items.clear();
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
                cout << "Signup successful!\n";
            }
        
            bool login(const string& uname, const string& pwd) const {
                for (const auto& u : users) {
                    if (u.username == uname && u.password == pwd) {
                        cout << "Login successful!\n";
                        return true;
                    }
                }
                cout << "Invalid credentials.\n";
                return false;
            }
        
            bool noUsers() const {
                return users.empty();
            }
        };
int main() {

    return 0;
}