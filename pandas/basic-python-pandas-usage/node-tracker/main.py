# Write in procedural format, maybe convert with objects later
# Data will be centered on pandas
# We need program control flow
# Log and Logging System (Function)
# a menu system
    # Data view
    # Data entry
        # Data parsers/handlers to validate before saving
    # Data delete
    # Data edit
    # Save to CSV
    # Load from CSV

import pandas as pd
import os

data = {
    "node": [],
    "status": []
}

df = pd.DataFrame(data)

def clear_screen():
    if os.name == 'nt':
        os.system('cls')
    else:
        os.system('clear')

def menu_input():
    menu = input("Selection: ")
    match menu:
        case "1":
            view_data()
        case "2":
            menu_enter()
        case "3":
            menu_edit()
        case "4":
            menu_delete()
        case "5":
            menu_save()
        case "6":
            menu_load()
        case "q":
            menu_quit()
        case _:
            print("Invalid Selection")

def post_menu():
    menu = input("Press Enter to Return")
    match menu:
        case _:
            menu_print()

def view_data():
    print(df)
    post_menu()

def menu_enter():
    print("Data Entry\n")

    # print(df.columns.tolist())
    # print(len(df.columns))
    add_row = {}

    for column in df.columns:
        data = input("Enter " + column + ": ")
        add_row[column] = data

    print(add_row)
    df.loc[len(df)] = add_row

def menu_edit():
    global df
    print("Edit Entry\n")
    node_name = input("Enter node name: ")

    # search existing dataframes for column
    print(df[df["node"] == node_name])
    temp = df[df["node"] == node_name]

    selection = input("Would you like to edit y/n: ")

    match selection:
        case "y":
            
            df = df.drop(df[df["node"] == node_name].index)

            add_row = {}

            for column in df.columns:
                data = input("Enter " + column + ": ")
                add_row[column] = data
            
            df.loc[len(df)] = add_row

            print("Old: " + temp)
            print("New :" + df[df["node"] == node_name])

        case "n":
            print("Discarding Edit")
        case _:
            print("Invalid Selection")

def menu_delete():
    global df
    print("Delete a node by name")

    data = input("Enter node name: ")

    df = df[df['node'] != data]

    print("Node matching: " + data + " - has been deleted")

    menu = input("Press Enter to Return")
    match menu:
        case _:
            menu_print()

def menu_save():
    global df
    df.to_csv('nodes.csv', index=True)

    menu = input("File Saved, Press Enter to Return")
    match menu:
        case _:
            menu_print()

def menu_load():
    global df
    df = pd.read_csv('nodes.csv')

    menu = input("File Loaded, Press Enter to Return")
    match menu:
        case _:
            menu_print()


def menu_quit():
    quit()

def menu_print():
    #clear_screen()
    print("----Main Menu----")
    print("|1 - View Data  |")
    print("|2 - Enter Data |")
    print("|3 - Edit Data  |")
    print("|4 - Delete Data|")
    print("|5 - Save Data  |")
    print("|6 - Load Data  |")
    print("|q - Quit       |")
    print("-----------------")
    menu_input()

def main():
    # Main here
    print("Main started\n")
    control = 1
    
    while(control):
        menu_print()

if __name__ == "__main__":
    main()