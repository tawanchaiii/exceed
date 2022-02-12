from pymongo import MongoClient
client = MongoClient('mongodb://localhost',27017)
db = client["Exceed"]
menu_collection = db["test"]

mylist = client.list_database_names()

print(mylist)