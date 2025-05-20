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

int main() {
    return 0;
}