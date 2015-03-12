/**
 * Penlook Project
 *
 * Copyright (c) 2015 Penlook Development Team
 *
 * --------------------------------------------------------------------
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * Authors:
 *     Loi Nguyen       <loint@penlook.com>
 *     Tin Nguyen       <tinntt@penlook.com>
 *     Nam Vo           <namvh@penlook.com>
 */

namespace Phalcon\Driver\Redis;

class Redis {

	/**
     * Redis static instance
     *
     * @var Phalcon\Driver
     */
	private static static_redis;

	/**
	 * Redis connection
	 */
	private connection;
	private setkey;

    /**
     * Constructor
     *
     */
    private function __construct()
	{
		print_r(this->connect());
		print_r("\n");
	}

	public static function getInstance()
	{
		if ! self::static_redis {
            let self::static_redis = new Redis();
        }

        return self::static_redis;
	}

	public function connect(host = "127.0.0.1", port = "6379")
	{
		var connection;
		let connection = redis("connect", host, port);
		var_dump(connection);
		var result;
		let result = redis("set", connection, "value", "value2");
		return result;
	}

	public function getConnection()
	{
		return this->connection;
	}

	public function getKey()
	{
		return this->setkey;
	}

	public function set(key, value)
	{
	}

	public function get(key)
	{
	}
}