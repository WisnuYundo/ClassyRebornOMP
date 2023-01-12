//function dan callback

stock OnMySQLConnect()
{
	new MySQLOpt: option_id = mysql_init_options();

	mysql_set_option(option_id, AUTO_RECONNECT, true); // it automatically reconnects when loosing connection to mysql server

	g_SQL = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, option_id); // AUTO_RECONNECT is enabled for this connection handle only
	if (g_SQL == MYSQL_INVALID_HANDLE || mysql_errno(g_SQL) != 0)
	{
		print("Koneksi MySQL gagal terhubung.Server dimatikan.");
		SendRconCommand("exit"); // close the server if there is no connection
		return 1;
	}

	print("MySQL Berhasil terhubung!.");
	return 1;
}