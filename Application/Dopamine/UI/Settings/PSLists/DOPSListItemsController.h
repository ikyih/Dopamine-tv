//
//  DOPSListItemsController.h
//  Dopamine
//
//  Created by tomt000 on 26/01/2024.
//

#import <UIKit/UIKit.h>
#import <TargetConditionals.h>
#if !TARGET_OS_TV
#import <Preferences/PSListController.h>
#import <Preferences/PSSpecifier.h>
#endif


NS_ASSUME_NONNULL_BEGIN

#if TARGET_OS_TV
@interface DOPSListItemsController : UIViewController
+ (UIView *)makeHeader:(NSString *)title withTarget:(id)target;
@end
#else
@interface PSListItemsController : PSListController
- (id)itemsFromDataSource;
@end

@interface DOPSListItemsController : PSListItemsController

+ (UIView *)makeHeader:(NSString *)title withTarget:(id)target;

@end
#endif

NS_ASSUME_NONNULL_END
