// Advanced C++ - Polymorphism with Containers
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cmath>      // For sqrt in triangle area calculation
#include <cstdlib>    // For rand() and srand()

/*
 * POLYMORPHISM WITH CONTAINERS
 * 
 * This example demonstrates how to use polymorphism effectively with containers in C++.
 * Key concepts demonstrated:
 * 
 * 1. Storing different derived types in a container of base class pointers
 * 2. Using virtual functions to achieve runtime polymorphic behavior
 * 3. Managing memory with smart pointers in containers
 * 4. Processing heterogeneous collections uniformly
 */

// Base class defining the interface for all shapes
class Shape {
protected:
    std::string name;
    std::string color;

public:
    // Constructor
    Shape(std::string name, std::string color) 
        : name(std::move(name)), color(std::move(color)) {}
    
    // Pure virtual function for calculating area - makes Shape abstract
    virtual double calculateArea() const = 0;
    
    // Pure virtual function for calculating perimeter
    virtual double calculatePerimeter() const = 0;
    
    // Virtual function for drawing the shape
    virtual void draw() const {
        std::cout << "Drawing a " << color << " " << name << std::endl;
    }
    
    // Getter for the name
    std::string getName() const { return name; }
    
    // Getter for the color
    std::string getColor() const { return color; }
    
    // Virtual destructor is essential for proper cleanup of derived classes
    virtual ~Shape() {
        std::cout << "Destroying " << color << " " << name << std::endl;
    }
};

// Circle class - derived from Shape
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double radius, std::string color)
        : Shape("Circle", std::move(color)), radius(radius) {}
    
    double calculateArea() const override {
        return 3.14159 * radius * radius;
    }
    
    double calculatePerimeter() const override {
        return 2 * 3.14159 * radius;
    }
    
    void draw() const override {
        Shape::draw();
        std::cout << "  Radius: " << radius << std::endl;
        std::cout << "  (O)" << std::endl;
    }
};

// Rectangle class - derived from Shape
class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double width, double height, std::string color)
        : Shape("Rectangle", std::move(color)), width(width), height(height) {}
    
    double calculateArea() const override {
        return width * height;
    }
    
    double calculatePerimeter() const override {
        return 2 * (width + height);
    }
    
    void draw() const override {
        Shape::draw();
        std::cout << "  Width: " << width << ", Height: " << height << std::endl;
        std::cout << "  [Rectangle]" << std::endl;
    }
};

// Triangle class - derived from Shape
class Triangle : public Shape {
private:
    double a, b, c;  // Sides of the triangle

public:
    Triangle(double a, double b, double c, std::string color)
        : Shape("Triangle", std::move(color)), a(a), b(b), c(c) {}
    
    double calculateArea() const override {
        // Using Heron's formula
        double s = (a + b + c) / 2;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }
    
    double calculatePerimeter() const override {
        return a + b + c;
    }
    
    void draw() const override {
        Shape::draw();
        std::cout << "  Sides: " << a << ", " << b << ", " << c << std::endl;
        std::cout << "   /\\" << std::endl;
        std::cout << "  /__\\" << std::endl;
    }
};

// Simple function to create random shapes
std::unique_ptr<Shape> createRandomShape() {
    // Random values between 1.0 and 10.0
    auto randomDouble = []() -> double {
        return 1.0 + (rand() % 90) / 10.0;
    };
    
    // Simple array of colors
    const std::string colors[] = {"Red", "Blue", "Green", "Yellow", "Purple"};
    
    // Pick a random color
    std::string color = colors[rand() % 5];
    
    // Randomly select shape type (0-2)
    int shapeType = rand() % 3;
    
    switch (shapeType) {
        case 0: // Circle
            return std::make_unique<Circle>(randomDouble(), color);
            
        case 1: // Rectangle
            return std::make_unique<Rectangle>(randomDouble(), randomDouble(), color);
            
        case 2: { // Triangle - using block to avoid switch case crossing initialization error
            // Create random sides that can form a valid triangle
            double a = randomDouble();
            double b = randomDouble();
            
            // c must be less than a+b but greater than |a-b| to form a valid triangle
            double minC = std::abs(a - b) + 0.1;
            double maxC = a + b - 0.1;
            double c = minC + (rand() % static_cast<int>((maxC - minC) * 10)) / 10.0;
            
            return std::make_unique<Triangle>(a, b, c, color);
        }
        
        default:
            return std::make_unique<Circle>(randomDouble(), color);
    }
}

// Function to analyze a collection of shapes
void analyzeShapes(const std::vector<std::unique_ptr<Shape>>& shapes) {
    // Count shapes by type
    int circles = 0, rectangles = 0, triangles = 0;
    
    // Find shapes with max and min areas
    double maxArea = 0.0;
    double minArea = 999999.0;
    const Shape* maxAreaShape = nullptr;
    const Shape* minAreaShape = nullptr;
    
    // Calculate total area and perimeter
    double totalArea = 0.0;
    double totalPerimeter = 0.0;
    
    // Process each shape polymorphically
    for (const auto& shape : shapes) {
        // Determine shape type and count
        if (dynamic_cast<const Circle*>(shape.get())) {
            circles++;
        } else if (dynamic_cast<const Rectangle*>(shape.get())) {
            rectangles++;
        } else if (dynamic_cast<const Triangle*>(shape.get())) {
            triangles++;
        }
        
        // Calculate area and update max/min
        double area = shape->calculateArea();
        totalArea += area;
        
        if (area > maxArea) {
            maxArea = area;
            maxAreaShape = shape.get();
        }
        
        if (area < minArea) {
            minArea = area;
            minAreaShape = shape.get();
        }
        
        // Add to total perimeter
        totalPerimeter += shape->calculatePerimeter();
    }
    
    // Display results
    std::cout << "\n===== Shape Analysis =====" << std::endl;
    std::cout << "Total shapes: " << shapes.size() << std::endl;
    std::cout << "  Circles: " << circles << std::endl;
    std::cout << "  Rectangles: " << rectangles << std::endl;
    std::cout << "  Triangles: " << triangles << std::endl;
    
    std::cout << "\nTotal area: " << totalArea << std::endl;
    std::cout << "Average area: " << (shapes.empty() ? 0 : totalArea / shapes.size()) << std::endl;
    
    std::cout << "\nTotal perimeter: " << totalPerimeter << std::endl;
    std::cout << "Average perimeter: " << (shapes.empty() ? 0 : totalPerimeter / shapes.size()) << std::endl;
    
    if (maxAreaShape) {
        std::cout << "\nLargest shape: " << maxAreaShape->getColor() << " " << maxAreaShape->getName() 
                 << " (Area: " << maxArea << ")" << std::endl;
    }
    
    if (minAreaShape) {
        std::cout << "Smallest shape: " << minAreaShape->getColor() << " " << minAreaShape->getName() 
                 << " (Area: " << minArea << ")" << std::endl;
    }
}

// Custom function object for sorting shapes by area
struct SortByArea {
    bool operator()(const std::unique_ptr<Shape>& a, const std::unique_ptr<Shape>& b) const {
        return a->calculateArea() < b->calculateArea();
    }
};

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    std::cout << "===== Polymorphism with Containers Example =====" << std::endl;
    
    // Create a vector to store different types of shapes
    std::vector<std::unique_ptr<Shape>> shapes;
    
    // Create a collection of random shapes
    std::cout << "\nCreating random shapes..." << std::endl;
    const int NUM_SHAPES = 10;
    
    for (int i = 0; i < NUM_SHAPES; ++i) {
        shapes.push_back(createRandomShape());
    }
    
    // Display all shapes (demonstrating polymorphic behavior)
    std::cout << "\n===== All Shapes =====" << std::endl;
    for (const auto& shape : shapes) {
        shape->draw();
        std::cout << "  Area: " << shape->calculateArea() << std::endl;
        std::cout << "  Perimeter: " << shape->calculatePerimeter() << std::endl;
        std::cout << std::endl;
    }
    
    // Analyze the collection
    analyzeShapes(shapes);
    
    // Sort shapes by area (demonstrating use of function objects with containers)
    std::cout << "\n===== Shapes Sorted by Area =====" << std::endl;
    std::sort(shapes.begin(), shapes.end(), SortByArea());
    
    for (const auto& shape : shapes) {
        std::cout << shape->getColor() << " " << shape->getName() 
                 << " - Area: " << shape->calculateArea() << std::endl;
    }
    
    // Demonstrate filtering with a lambda and another container
    std::cout << "\n===== Red Shapes =====" << std::endl;
    std::vector<const Shape*> redShapes;
    
    for (const auto& shape : shapes) {
        if (shape->getColor() == "Red") {
            redShapes.push_back(shape.get());
        }
    }
    
    std::cout << "Found " << redShapes.size() << " red shapes:" << std::endl;
    for (const auto* shape : redShapes) {
        std::cout << shape->getName() << " - Area: " << shape->calculateArea() << std::endl;
    }
    
    std::cout << "\nEnd of program - all shapes will be automatically deleted" << std::endl;
    
    // No explicit cleanup needed - smart pointers handle deletion
    return 0;
} 