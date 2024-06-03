import turtle
import time  # Import time module for sleep

class Node:
    """Binary tree node with visualization coordinates."""
    def __init__(self, data, x=0, y=0):
        self.data = data
        self.left = None
        self.right = None
        self.x = x
        self.y = y

def insert(root, key, x, y, distance):
    """Insert key into the binary tree at the first position available and visualize it."""
    if root is None:
        root = Node(key, x, y)
        draw_node(root, key)
        return root
    else:
        if key < root.data:
            if root.left is None:
                root.left = Node(key, x - distance, y - 100)
                draw_node(root.left, key)
                draw_connection(root, root.left)
            else:
                root.left = insert(root.left, key, x - distance, y - 100, distance // 2)
        else:
            if root.right is None:
                root.right = Node(key, x + distance, y - 100)
                draw_node(root.right, key)
                draw_connection(root, root.right)
            else:
                root.right = insert(root.right, key, x + distance, y - 100, distance // 2)
        return root

def draw_node(node, key):
    """Draw a single node on the screen."""
    turtle.penup()
    turtle.goto(node.x, node.y)
    turtle.pendown()
    turtle.circle(20)
    turtle.penup()
    turtle.goto(node.x, node.y - 10)
    turtle.pendown()
    turtle.write(key, align="center", font=("Arial", 12, "normal"))
    turtle.penup()

def draw_connection(parent, child):
    """Draw a line between parent and child nodes."""
    turtle.penup()
    turtle.goto(parent.x, parent.y - 20)
    turtle.pendown()
    turtle.goto(child.x, child.y + 20)
    turtle.penup()

def count_nodes(node):
    """Recursive function to count nodes, with visualization of the recursion process."""
    if node is None:
        return 0
    turtle.goto(node.x, node.y)
    turtle.dot(20, "blue")  # Highlight the current node for counting
    time.sleep(0.5)  # Pause to observe the counting
    left_count = count_nodes(node.left)
    right_count = count_nodes(node.right)
    total_count = 1 + left_count + right_count
    display_count(node, total_count)  # Display the accumulated count at the node
    return total_count

def display_count(node, count):
    """Display the accumulated count near the node."""
    turtle.goto(node.x, node.y + 30)
    turtle.write(str(count), align="center", font=("Arial", 10, "normal"))
    turtle.penup()

def setup_turtle():
    """Setup turtle graphics settings."""
    turtle.speed(1)
    turtle.penup()
    turtle.hideturtle()

# Main program to create the tree and count nodes
setup_turtle()
root = None
nodes = [10, 5, 20, 3, 7, 15]
x, y = 0, 300  # Initial position of the root
distance = 200  # Initial distance for horizontal placement

# Insert nodes into the tree
for key in nodes:
    root = insert(root, key, x, y, distance)

print("Tree created")
# Don't call turtle.done() here to allow counting to proceed
print("Starting to count nodes...")
count = count_nodes(root)
print("Total nodes:", count)
turtle.done()  # Call turtle.done() only after all operations are complete
