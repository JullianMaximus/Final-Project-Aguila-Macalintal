#include <iostream>
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
int main() {
    return 0;
}