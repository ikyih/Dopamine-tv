//
//  DOPSExploitListItemsControllerViewController.h
//  Dopamine
//
//  Created by Lars Fröder on 29.04.24.
//

#import "DOPSListItemsController.h"
#import <TargetConditionals.h>

#if !TARGET_OS_TV
NS_ASSUME_NONNULL_BEGIN

@interface DOPSJetsamListItemsController : DOPSListItemsController

@end

NS_ASSUME_NONNULL_END
#endif
