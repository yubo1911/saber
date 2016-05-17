#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address{
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database{
	struct Address rows[MAX_ROWS];
};

struct Connection{
	FILE *file;
	struct Database *db;
};

void die(const char *message)
{
	if (errno)
	{
		perror(message);
	}
	else{
		printf("ERROR: %s.\n", message);
	}

	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", 
			addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1)
	{
		die("Failed to load database.\n");
	}
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn)
	{
		die("Memory error.\n");
	}

	conn->db = malloc(sizeof(struct Database));
	if (!conn->db)
	{
		die("Memory error.\n");
	}

	if (mode == 'c')
	{
		conn->file = fopen(filename, "w");
	}
	else
	{
		conn->file = fopen(filename, "r+");
		if (conn->file)
		{
			Database_load(conn);
		}
	}

	if (!conn->file)
	{
		die("Fail to open the file.\n");
	}

	return conn;
}

void Database_close(struct Connection *conn)
{
	if (conn)
	{
		if (conn->file)
		{
			fclose(conn->file);
		}
		if (conn->db)
		{
			free(conn->db);
		}
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1)
	{
		die("Failed to write database.\n");
	}

	rc = fflush(conn->file);
	if (rc == -1)
	{
		die("Cannot fflush database.\n");
	}
}

void Database_create(struct Connection *conn)
{
	int i = 0;

	for (i=0; i<MAX_ROWS; ++i)
	{
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct  Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if (addr->set)
	{
		die("Already set. Delete it first.\n");
	}

	addr->set = 1;
	char *res = strncpy(addr->name, name, MAX_DATA);
	if (!res)
	{
		die("Name copy failed.\n");
	}

	res = strncpy(addr->email, email, MAX_DATA);
	if (!res)
	{
		die("Email copy failed.\n");
	}
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];

	if (addr->set)
	{
		Address_print(addr);
	}
	else
	{
		die("ID is not set.\n");
	}
}

void Database_delete(struct Connection *conn, int id)
{
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for (i=0; i<MAX_ROWS; ++i)
	{
		struct Address *addr = &db->rows[i];

		if (addr->set)
		{
			Address_print(addr);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		die("Usage: ex17 <dbfile> <action> [action params].\n");
	}

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	
	int id = 0;
	if (argc > 3)
	{
		id = atoi(argv[3]);
	}

	if (id >= MAX_ROWS)
	{
		die("There's not that many records.\n");
	}

	switch(action)
	{
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;

		case 'g':
			if (argc != 4)
			{
				die("Need an id to get.\n");
			}
			Database_get(conn, id);
			break;

		case 's':
			if (argc != 6)
			{
				die("Need id name email to set.\n");
			}

			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;

		case 'd':
			if (argc != 4)
			{
				die("Need an id to delete.\n");
			}

			Database_delete(conn, id);
			Database_write(conn);
			break;

		case 'l':
			Database_list(conn);
			break;

		default:
			die("Invalid action, only: c=create, g=get, s=set, d=delete, l=list.\n");
	}

	Database_close(conn);

	return 0;
}
