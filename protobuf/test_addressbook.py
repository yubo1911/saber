# -*- coding: utf-8 -*-

from proto import addressbook_pb2

addressbook = addressbook_pb2.AddressBook()

person = addressbook.person.add()
person.id = 1
person.name = "yubo"
person.email = "yubo1911@163.com"
phone = person.phone.add()
phone.number = "13345678901"
phone.type = addressbook_pb2.Person.HOME

person2 = addressbook.person.add()
person2.id = 2
person2.name = "usher"
person2.email = "usher@163.com"
phone2 = person2.phone.add()
phone2.number = "13345078901"
phone2.type = addressbook_pb2.Person.HOME

with open("serialized.txt", "wb") as f:
    f.write(addressbook.SerializeToString())

addressbook2 = addressbook_pb2.AddressBook()
with open("serialized.txt", "rb") as f:
    addressbook2.ParseFromString(f.read())

print(addressbook2.person[0].name)
