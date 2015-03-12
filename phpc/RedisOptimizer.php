<?php
/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@phalconphp.com so we can send you a copy immediately.       |
 +------------------------------------------------------------------------+
 | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
 |          Eduar Carvajal <eduar@phalconphp.com>                         |
 +------------------------------------------------------------------------+
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;
use Zephir\HeadersManager;

class RedisOptimizer extends OptimizerAbstract
{
	public function getParams($expression)
	{
		$params = [];
		$mixs = $expression['parameters'];

		foreach ($mixs as $arg) {
			if ($arg["parameter"]["value"] == NULL) {
				print($arg["parameter"]);
			} else {
				$params[] = $arg["parameter"]["value"];
			}
		}

		return $params;
	}

	/**
	 *
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{
        $call->processExpectedReturn($context);
      	$args = $this->getParams($expression);
        $func = strtolower($args[0]);

        $call->processExpectedReturn($context);

		$symbolVariable = $call->getSymbolVariable();
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
		}

		if ($call->mustInitSymbolVariable()) {
			$symbolVariable->initVariant($context);
		}

        $args = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        switch ($func) {
        	case 'connect':
        		$other_arguments = $args[1] . ','. $args[2];
        		break;

        	default:
        		$other_arguments = $args[1] . ','. $args[2] .','. $args[3];
        		break;
        }

        $cmd = 'redis_' . $func .'('. $symbolVariable->getName(). ',' . $other_arguments . ');';
        echo $cmd;

        $context->codePrinter->output($cmd);
        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}