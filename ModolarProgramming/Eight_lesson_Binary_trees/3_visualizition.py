

import turtle
import time


# Constants
SCREEN_WIDTH = 1000
SCREEN_HEIGHT = 800
INITIAL_X = 0
INITIAL_Y = 300
INITIAL_DISTANCE = 200
CIRCLE_RADIUS = 20
FONT_STYLE = ("Arial", 12, "normal")
NULL_LINE_LENGTH = 10  # Shorter length for null child indicators
SLEEP_TIME = 1
COUNT_FONT_STYLE = ("Arial", 14, "bold")




# Global flag for pausing
paused = False

def toggle_pause():
    """Toggle the global pause flag."""
    global paused
    paused = not paused

def wait_for_resume():
    """Wait loop that continues until paused is False."""
    while paused:
        turtle.update()
        time.sleep(0.1)  # Check every 100ms if still paused



class Node:
    """ Represents a binary tree node. """
    def __init__(self, data, x=0, y=0):
        self.data = data
        self.left = None
        self.right = None
        self.x = x
        self.y = y

def init_screen():
    """ Initializes the turtle screen. """
    turtle.setup(SCREEN_WIDTH, SCREEN_HEIGHT)
    turtle.title("Binary Tree Visualization")
  
    turtle.bgcolor("#f2f2f2")
def insert(root, key, x, y, distance):
    """ Inserts a key into the binary tree at the first position available. """
    wait_for_resume()
    if root is None:
        root = Node(key, x, y)
        draw_node(root, key)
        return root
    elif key < root.data:
        if root.left is None:
            root.left = insert(None, key, x - distance, y - 100, distance // 2)
            draw_connection(root, root.left, False)
        else:
            insert(root.left, key, x - distance, y - 100, distance // 2)
    else:
        if root.right is None:
            root.right = insert(None, key, x + distance, y - 100, distance // 2)
            draw_connection(root, root.right, False)
        else:
            insert(root.right, key, x + distance, y - 100, distance // 2)
    return root

def draw_node(node, key):
    wait_for_resume()  # Global pause check
    """ Draws a node at its position with the specified key. """
    turtle.penup()
    turtle.goto(node.x, node.y)
    turtle.pendown()
    turtle.circle(CIRCLE_RADIUS)
    turtle.penup()
    turtle.goto(node.x, node.y + CIRCLE_RADIUS*2)
    turtle.pendown()
    turtle.write(key, align="center", font=FONT_STYLE)
    turtle.penup()

def draw_connection(parent, child, is_null):
    wait_for_resume()  # Global pause check
    """ Draws a connection from a parent to its child. If is_null, draw a null indicator. """
    turtle.penup()
    start_x = parent.x 
    start_y = parent.y 
    end_x = child.x if not is_null else start_x + (NULL_LINE_LENGTH if child == "left" else -NULL_LINE_LENGTH)
    end_y = child.y + CIRCLE_RADIUS if not is_null else start_y - 40
    turtle.goto(start_x, start_y)
    turtle.pendown()
    turtle.goto(end_x, end_y)
    turtle.penup()

def finalize_nulls(node):
    """ After all insertions, draw null indicators for leaf nodes. """
    if node is not None:
        if node.left is None:
            draw_connection(node, "left", True)
        else:
            finalize_nulls(node.left)
        if node.right is None:
            draw_connection(node, "right", True)
        else:
            finalize_nulls(node.right)

def setup_turtle():
    """ Configures turtle graphics settings. """
    turtle.speed(1)
    turtle.penup()
    turtle.hideturtle()
    turtle.listen()  # Listen for keyboard input
    turtle.onkey(toggle_pause, "p")  # Bind 'p' key to toggle pause




def count_nodes(node):
    """ Counts nodes recursively with visual feedback on the process. """
    wait_for_resume()  # Global pause check
    if node is None:
        return 0
    turtle.goto(node.x, node.y)
    turtle.dot(CIRCLE_RADIUS, "blue")
    time.sleep(SLEEP_TIME)
    left_count = count_nodes(node.left)
    right_count = count_nodes(node.right)
    total_count = 1 + left_count + right_count
    display_count(node, total_count)
    recursion_finished(node)
    return total_count

def display_count(node, count):
    """ Displays the count of nodes accumulated at a node. """
    wait_for_resume()  # Global pause check
    turtle.goto(node.x, node.y+10 )
    turtle.write(str(count), align="center", font=COUNT_FONT_STYLE)
    turtle.penup()
def recursion_finished(node):
    """ Check if the recursion has finished for a node. """
    wait_for_resume()  # Global pause check
    turtle.goto(node.x, node.y)
    turtle.dot(CIRCLE_RADIUS, "green")
    time.sleep(SLEEP_TIME)
    

def display_pause_message():
    """ Display a pause message on the screen. """
    turtle.goto(-450,SCREEN_HEIGHT//3)
    turtle.write("PRESS P TO PAUSE", align="left", font=FONT_STYLE)
    turtle.goto(-450,SCREEN_HEIGHT//3 - 20)
    turtle.write("PRESS P AGAIN TO RELEASE", align="left", font=FONT_STYLE)
# Main execution block to create the tree


init_screen()
setup_turtle()
display_pause_message()
root = None
nodes = [10, 5, 20, 3, 7, 15]
for key in nodes:
    root = insert(root, key, INITIAL_X, INITIAL_Y, INITIAL_DISTANCE)

finalize_nulls(root)  # Draw null indicators only after all nodes are inserted
count = count_nodes(root)
print("Total nodes:", count)
turtle.done()  # Conclude turtle operations