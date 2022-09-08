--Create a new table for history of all actions, bought&sold stocks
CREATE TABLE actions (
    transaction_id int,
    symbol TEXT NOT NULL,
    shares int NOT NULL,
    price int NOT NULL,
    action_type TEXT NOT NULL,
    time_of DATETIME NOT NULL,
    person_id int,
    PRIMARY KEY(transaction_id),
    FOREIGN KEY(person_id) REFERENCES users(id)
);